#!/usr/bin/env python3

import sqlite3

print('creating database...')

conn = sqlite3.connect('freeachtung.db')
c = conn.cursor()

c.execute("CREATE TABLE \"Players\"(\
    [id] INTEGER PRIMARY KEY NOT NULL,\
    [name] NVARCHAR(20) NOT NULL,\
    [color] INTEGER NOT NULL\
)")

c.execute("INSERT INTO Players\
    VALUES\
        (1, 'test', 16777215),\
        (2, 'another', 3881787);")

conn.commit()
conn.close()
