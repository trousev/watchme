select
    count(timestamp) as rec_count,
    watch.wtitle as wtitle,
    watch.wclass as wclass,
    rules.pattern as pattern
from
    (select * from watch as watch LIMIT 10000) as watch
            LEFT JOIN
    (SELECT * FROM rules order by level DESC) as rules
            ON  watch.wclass LIKE rules.wclass
            AND watch.wtitle LIKE rules.wtitle

group by
    watch.wtitle, watch.wclass;
