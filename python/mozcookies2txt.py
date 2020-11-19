#!/usr/bin/env python

import sqlite3 as db

cookie_file = r'cookies.sqlite'
output_file = r'cookies.txt'

conn = db.connect(cookie_file)
cur = conn.cursor()
cur.execute('SELECT host, path, isSecure, expiry, name, value FROM moz_cookies')
f = open(output_file, 'w')
i = 0
for row in cur.fetchall():
    f.write("%s\tTRUE\t%s\t%s\t%d\t%s\t%s\n" % (row[0], row[1], str(bool(row[2])).upper(), row[3], str(row[4]), str(row[5])))
i += 1
print "%d rows written" % i
f.close()
conn.close()
