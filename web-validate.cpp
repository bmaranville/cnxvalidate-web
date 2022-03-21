#include <iostream>
#include <string>

#include <emscripten/bind.h>
#include <emscripten.h>
#include <nxvalidate.h>

using namespace emscripten;

struct MessageList
{
    val messages = val::undefined();
    unsigned int length;
};

void hash_enumerate(hash_table *table, val message)
{
    unsigned i;
    bucket *temp;
    bucket *swap;

    for (i = 0; i < table->size; i++)
    {
        if (NULL != (table->table)[i])
        {
            /* HW: changed this loop */
            temp = (table->table)[i];
            while (NULL != temp)
            {
                /* HW: swap trick, in case temp is freed by 'func' */
                swap = temp->next;
                message.set(std::string(temp->key), std::string((char *)(temp->data)));
                temp = swap;
            }
        }
    }
}

static void MessageLogger(hash_table *logData, void *userData)
{
    MessageList *message_list = (MessageList *)userData;
    val message = val::object();
    hash_enumerate(logData, message);
    message_list->messages.set(message_list->length++, message);
}

class Validator
{
    std::string defDir;
    pNXVcontext nxvContext;

public:
    Validator(const std::string &defDir_in);
    val validate(const std::string &filename, const std::string &appDef, const std::string &hdf5Path, bool procroot);
};

Validator::Validator(const std::string &defDir_in)
{
    defDir = defDir_in;
};

val Validator::validate(const std::string &filename, const std::string &appDef, const std::string &hdf5Path, bool procroot)
{
    
    MessageList message_list;
    message_list.messages = val::array();
    message_list.length = 0;

    nxvContext = NXVinit(defDir.c_str());
    if (nxvContext == NULL)
    {
        fprintf(stdout, "ERROR: failed to allocate validation context\n");
        exit(1);
    }
    const char *appDef_c = (appDef.empty()) ? NULL : appDef.c_str();
    const char *hdf5Path_c = (hdf5Path.empty()) ? NULL : hdf5Path.c_str();
    NXVsetLogger(nxvContext, MessageLogger, &message_list);
    int status = NXVvalidate(nxvContext, filename.c_str(), appDef_c, hdf5Path_c, (int)procroot);
    return message_list.messages;
};

// Binding code
EMSCRIPTEN_BINDINGS(validator)
{
    class_<Validator>("Validator")
        .constructor<std::string>()
        .function("validate", &Validator::validate);
}