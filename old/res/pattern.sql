select
    cast(count(timestamp) as DOUBLE) / 10 as rec_count,
    rules.pattern as pattern
from
    (select * from watch
            where timestamp > strftime('%s','now')-{minimum}
            and   timestamp < strftime('%s','now')-{maximum}
        ) as watch
            LEFT JOIN
    (SELECT * FROM rules order by level DESC) as rules
            ON  watch.wclass LIKE '%'|| rules.wclass || '%'
            AND watch.wtitle LIKE '%'|| rules.wtitle || '%'

group by
    rules.pattern;
