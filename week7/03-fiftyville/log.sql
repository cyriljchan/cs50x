-- Keep a log of any SQL queries you execute as you solve the mystery.

-- DESC: Scanning reports for basic description of the crime (CS50 Duck Theft)
-- RESULT: id-295 (2023-07-28 10:15am at the humphrey street bakery)
SELECT * FROM crime_scene_reports

-- DESC: Scanning interviews on the same day the crime happened
-- RESULT:  id-161 (Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.)
    --      id-162 (Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.)
    --      id-163 (they called someone who talked to them for less than a minute, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow)
SELECT * FROM interviews
    WHERE year == 2023 and month == 7 and day == 28;

-- DESC: Get suspects from bakery logs leaving 10min of the crime
-- RESULT: Set of plates leaving the bakery 10min of the crime
SELECT * FROM bakery_security_logs
    WHERE year == 2023 AND month == 7 AND day == 28
        AND hour == 10 AND minute >= 15 AND minute <=25
        AND activity == "exit";

-- DESC: Get suspects from phone logs on the day for less than 60s
-- RESULT: Set of phone logs on the day
SELECT * FROM phone_calls
    WHERE year == 2023 AND month == 7 AND day == 28
        AND duration < 60;

-- DESC: Get suspects from bank logs
-- RESULT: Set of bank logs on the day
SELECT * FROM atm_transactions as atm
    INNER JOIN bank_accounts as ba ON atm.account_number == ba.account_number
    WHERE year == 2023 AND month == 7 AND day == 28
        AND atm_location == "Leggett Street" AND transaction_type == "withdraw";

-- DESC: Obtain final list by finding  the lists
-- RESULT: Final list of suspects
-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | 514354 | Diana | (770) 555-1861 | 3592750733      | 322W7JE       |
-- +--------+-------+----------------+-----------------+---------------+
SELECT p.id, p.name, p.phone_number, p.passport_number, p.license_plate FROM people as p
    INNER JOIN
        (SELECT * FROM bakery_security_logs
            WHERE year == 2023 AND month == 7 AND day == 28
                AND hour == 10 AND minute >= 15 AND minute <=25
                AND activity == "exit") AS bsl
        ON p.license_plate == bsl.license_plate

    INNER JOIN
        (SELECT * FROM phone_calls
            WHERE year == 2023 AND month == 7 AND day == 28
                AND duration < 60) AS calls
        ON p.phone_number == calls.caller

    INNER JOIN
        (SELECT * FROM atm_transactions as atm
            INNER JOIN bank_accounts as ba ON atm.account_number == ba.account_number
            WHERE year == 2023 AND month == 7 AND day == 28
                AND atm_location == "Leggett Street" AND transaction_type == "withdraw") AS atm
        ON p.id == atm.person_id;

-- DESC: Obtain earliest flight out of Fiftyville the next day
-- RESULT: 2023-7-29 8:20 Going to New York City
-- +----+-------------------+------------------------+------+-------+-----+------+--------+----+--------------+-----------------------------+------------+
-- | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute | id | abbreviation |          full_name          |    city    |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+----+--------------+-----------------------------+------------+
-- | 36 | 8                 | 4                      | 2023 | 7     | 29  | 8    | 20     | 8  | CSF          | Fiftyville Regional Airport | Fiftyville |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+----+--------------+-----------------------------+------------+
SELECT * FROM flights
    INNER JOIN airports ON flights.origin_airport_id == airports.id
    WHERE airports.city == "Fiftyville"
        AND flights.year == 2023 AND flights.month == 7 AND flights.day == 29
        ORDER BY flights.hour, flights.minute;

-- DESC: Obtain thief by crossing final suspect list with passenger list
-- RESULT: The thief is Bruce
-- +-----------+-----------------+------+--------+-------+----------------+-----------------+---------------+
-- | flight_id | passport_number | seat |   id   | name  |  phone_number  | passport_number | license_plate |
-- +-----------+-----------------+------+--------+-------+----------------+-----------------+---------------+
-- | 36        | 5773159633      | 4A   | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +-----------+-----------------+------+--------+-------+----------------+-----------------+---------------+
SELECT * FROM passengers
    INNER JOIN people ON passengers.passport_number == people.passport_number
    WHERE passengers.flight_id == 36
        AND (people.passport_number == 5773159633 OR people.passport_number == 3592750733);

-- DESC: Get accomplice name from call log
-- RESULT: The accomplice is Robin
-- +-------+----------------+-----------------+
-- | name  |  phone_number  | passport_number |
-- +-------+----------------+-----------------+
-- | Robin | (375) 555-8161 | NULL            |
-- +-------+----------------+-----------------+
SELECT people.name, people.phone_number, people.passport_number FROM phone_calls
    INNER JOIN people ON phone_calls.receiver == people.phone_number
    WHERE year == 2023 AND month == 7 AND day == 28
        AND duration < 60
        AND caller == "(367) 555-5533";
