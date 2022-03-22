import requests
import io
import zipfile
import re
import os
import json

applications_store = "application_defs"
releases = requests.get("https://api.github.com/repos/nexusformat/definitions/releases").json()
applications_re = re.compile(r"([^/]+)/(applications|base_classes)/.*")
root_re = re.compile
version_lookups = {}

for release in releases:
    zipball_url = release['zipball_url']
    #output_path = os.path.join(applications_store, release['tag_name'])
    output_path = applications_store
    os.makedirs(output_path, exist_ok=True)
    zipball = zipfile.ZipFile(io.BytesIO(requests.get(zipball_url).content))
    apps = [fn for fn in zipball.filelist if applications_re.match(fn.filename) is not None]
    root = applications_re.match(apps[0].filename)
    root_path = os.path.join(output_path, root.group(1))
    version_lookups[release['tag_name']] = root_path
    for app in apps:
        zipball.extract(app, path=output_path)


open("application_paths.json", "wt").write(json.dumps(version_lookups, indent=2))


