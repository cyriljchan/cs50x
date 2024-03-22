SELECT COUNT(movies.title) FROM movies
    INNER JOIN ratings ON movies.id == ratings.movie_id
    WHERE rating == 10;
