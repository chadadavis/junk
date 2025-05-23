#!/usr/bin/env python
from pprint import pprint
import re
def pvars(_extra:dict={}):
    """If you're inside a def, call this as: pvars(vars())"""
    _vars = { **globals(), **locals(), **_extra }
    pprint([ [k,_vars[k]] for k in _vars if re.match(r'[a-z]', k)])



# Python script to convert Google Maps JSON exports to GPX waypoints for OSMAnd

import json
import sys
from pathlib import Path

def parse_places(data):
    for feat in data["features"]:
        lat, lon = feat["geometry"]["coordinates"]
        props = feat.get("properties", {})
        # Try top-level name/address, else look in 'location'
        name = props.get("name") or props.get("location", {}).get("name", "")
        desc = props.get("address") or props.get("location", {}).get("address", "")
        if (lat == 0 and lon == 0) or not (name or desc):
            continue
        yield lat, lon, name, desc

def write_gpx(waypoints, out_path):
    with open(out_path, "w", encoding="utf-8") as f:
        f.write('<?xml version="1.0" encoding="UTF-8"?>\n')
        f.write('<gpx version="1.1" creator="gmaps2gpx.py" xmlns="http://www.topografix.com/GPX/1/1">\n')
        for lat, lon, name, desc in waypoints:
            f.write(f'  <wpt lat="{lat}" lon="{lon}">\n')
            if name: f.write(f'    <name>{name}</name>\n')
            if desc: f.write(f'    <desc>{desc}</desc>\n')
            f.write('  </wpt>\n')
        f.write('</gpx>\n')

def main():
    #  For each JSON file, eg "Saved Places.json" or "Labeled Places.json"
    data = json.load(sys.stdin)
    waypoints = list(parse_places(data))
    write_gpx(waypoints, sys.stdout.fileno())

if __name__ == "__main__":
    main()