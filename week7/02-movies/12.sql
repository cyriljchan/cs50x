SELECT movies.title FROM movies
    INNER JOIN stars ON movies.id == stars.movie_id
    INNER JOIN people ON stars.person_id == people.id
    WHERE name == "Bradley Cooper" OR name == "Jennifer Lawrence"
    GROUP BY title
    HAVING COUNT(title) > 1;
