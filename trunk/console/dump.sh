drush sql-dump | grep -vE 'INSERT INTO `(cache|cache_\w+|search_\w+|sessions|watchdog)`' > db.sql
