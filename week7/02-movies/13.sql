SELECT people.name FROM stars
    -- Inner join stars with table containing movies that Kevin Bacon (1958) starred in
    -- filter: Same movie_id and any star that is NOT Kevin Bacon (1958)
    INNER JOIN (SELECT movie_id, person_id FROM stars
        INNER JOIN people ON stars.person_id == people.id
        WHERE name == "Kevin Bacon" AND birth == 1958) as kb
        ON stars.movie_id == kb.movie_id AND stars.person_id != kb.person_id

    INNER JOIN people ON stars.person_id == people.id;
