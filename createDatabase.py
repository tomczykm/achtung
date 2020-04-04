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
        (1, 'Fred', 16711680),\
        (2, 'Greenlee', 65280),\
        (3, 'Pinkney', 16711935),\
        (4, 'Bluebell', 65535),\
        (5, 'Willem', 16744448),\
        (6, 'Greydon', 13421772);")

conn.commit()
conn.close()
