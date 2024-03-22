SELECT DISTINCT people.name FROM directors
    INNER JOIN people ON directors.person_id == people.id
    INNER JOIN ratings ON directors.movie_id == ratings.movie_id
    WHERE rating >= 9;
