use archdb; 

 CREATE TABLE IF NOT EXISTS `User` (
    `id` INT NOT NULL AUTO_INCREMENT,
    `first_name` VARCHAR(256) NOT NULL,
    `last_name` VARCHAR(256) NOT NULL,
    `login` VARCHAR(256) NOT NULL,
    `password` VARCHAR(256) NOT NULL,
    `email` VARCHAR(256) NOT NULL,
    `phone` VARCHAR(256) NOT NULL,
    PRIMARY KEY (`id`),KEY `fn` (`first_name`),KEY `ln` (`last_name`),
    UNIQUE KEY `unique_email` (`email`),
    UNIQUE KEY `unique_phone` (`phone`));

CREATE TABLE IF NOT EXISTS `Route` (
    `id_route` INT NOT NULL AUTO_INCREMENT,
    `id_user` INT(11) NOT NULL,
    `point_start` VARCHAR(256) NOT NULL,
    `point_end` VARCHAR(256) NOT NULL,
    PRIMARY KEY (`id_route`),
    FOREIGN KEY (`id_user`) REFERENCES `User` (`id`));

CREATE TABLE IF NOT EXISTS `Trip` (`id` INT NOT NULL AUTO_INCREMENT,
    `route_ID` INT(11) NOT NULL,
    `driver` int(11) NOT NULL,
#    `user` int(11) NOT NULL,
    `date_depart` VARCHAR(256) NOT NULL,
    `travel_conditions` VARCHAR(256) NULL,
    `price` int(5) NOT NULL,
    PRIMARY KEY (`id`),
 #   FOREIGN KEY (`user`) REFERENCES `User` (`id`),
    FOREIGN KEY (`route_ID`) REFERENCES `Route` (`id_route`)); 

CREATE TABLE IF NOT EXISTS `user_trip` (
    `user_id` INT(11) NOT NULL,
    `trip_id` INT(11) NOT NULL,
    FOREIGN KEY (`user_id`) REFERENCES `User` (`id`),
    FOREIGN KEY (`trip_id`) REFERENCES `Trip` (`id`)); 

INSERT INTO `User` (`first_name`, `last_name`, `email`, `phone`, `login`, `password`) VALUES
    ('John', 'Doe', 'john.doe@email.com', '123-456-7890', 'johndoe', 'password123'),
    ('Jane', 'Smith', 'jane.smith@email.com', '987-654-3210', 'janesmith', 'securepass'),
    ('Mike', 'Johnson', 'mike.johnson@email.com', '555-123-4567', 'mikejohnson', 'pass1234'),
    ('Emily', 'Williams', 'emily.williams@email.com', '111-222-3333', 'emilywilliams', 'qwerty'),
    ('David', 'Brown', 'david.brown@email.com', '444-555-6666', 'davidbrown', 'letmein'),
    ('Sarah', 'Miller', 'sarah.miller@email.com', '777-888-9999', 'sarahmiller', '12345678'),
    ('Daniel', 'Taylor', 'daniel.taylor@email.com', '333-111-4444', 'danieltaylor', 'password1'),
    ('Olivia', 'Anderson', 'olivia.anderson@email.com', '999-666-1111', 'oliviaanderson', 'pass123'),
    ('James', 'White', 'james.white@email.com', '222-333-4444', 'jameswhite', 'iloveyou'),
    ('Ava', 'Davis', 'ava.davis@email.com', '888-777-6666', 'avadavis', 'hello123'),
    ('Michael', 'Martinez', 'michael.martinez@email.com', '777-888-1111', 'michaelmartinez', 'welcome1'),
    ('Sophia', 'Clark', 'sophia.clark@email.com', '555-444-3333', 'sophiaclark', 'testpass'),
    ('William', 'Thomas', 'william.thomas@email.com', '222-111-5555', 'williamthomas', 'changeme'),
    ('Emma', 'Moore', 'emma.moore@email.com', '999-888-7777', 'emmamoore', 'password!'),
    ('Christopher', 'Walker', 'christopher.walker@email.com', '666-555-4444', 'chriswalker', 'p@ssword'),
    ('Isabella', 'Baker', 'isabella.baker@email.com', '333-444-5555', 'isabellabaker', 'secure1'),
    ('Liam', 'Young', 'liam.young@email.com', '777-666-5555', 'liamyoung', 'letmein!'),
    ('Mia', 'Hill', 'mia.hill@email.com', '222-333-1111', 'miahill', 'password123!'),
    ('Ethan', 'Ward', 'ethan.ward@email.com', '888-777-3333', 'ethanward', 'test1234'),
    ('Amelia', 'King', 'amelia.king@email.com', '444-555-7777', 'ameliaking', '123qwe');

INSERT INTO `Route` (`id_user`, `point_start`, `point_end`) VALUES
    (1, 'Москва', 'Санкт-Петербург'),
    (2, 'Санкт-Петербург', 'Екатеринбург'),
    (3, 'Новосибирск', 'Красноярск'),
    (4, 'Владивосток', 'Иркутск'),
    (5, 'Казань', 'Самара'),
    (6, 'Уфа', 'Пермь'),
    (7, 'Ростов-на-Дону', 'Волгоград'),
    (8, 'Краснодар', 'Сочи'),
    (9, 'Омск', 'Тюмень'),
    (10, 'Челябинск', 'Оренбург'),
    (11, 'Томск', 'Барнаул'),
    (12, 'Ульяновск', 'Саратов'),
    (13, 'Воронеж', 'Липецк'),
    (14, 'Ярославль', 'Кострома'),
    (15, 'Калининград', 'Киров'),
    (16, 'Ижевск', 'Нижний Новгород'),
    (17, 'Хабаровск', 'Благовещенск'),
    (18, 'Мурманск', 'Архангельск'),
    (19, 'Симферополь', 'Севастополь'),
    (20, 'Магадан', 'Анадырь');


INSERT INTO `Trip` (`route_ID`, `driver`, `date_depart`, `travel_conditions`, `price`) VALUES
    (1, 21, '2023-12-20', 'Не курить, без животных', 400),
    (2, 22, '2023-12-21', 'Без громкой музыки', 550),
    (3, 23, '2023-12-22', 'Люблю поговорить', 700),
    (4, 24, '2023-12-23', 'Без разговоров', 850),
    (5, 25, '2023-12-24', 'Люблю музыку', 1000),
    (6, 26, '2023-12-25', 'Не курить, без животных', 450),
    (7, 27, '2023-12-26', 'Без разговоров', 600),
    (8, 28, '2023-12-27', 'Люблю поговорить', 750),
    (9, 29, '2023-12-28', 'Без громкой музыки', 900),
    (10, 30, '2023-12-29', 'Люблю музыку', 1050),
    (11, 31, '2023-12-20', 'Без разговоров', 500),
    (12, 32, '2023-12-21', 'Не курить, без животных', 650),
    (13, 33, '2023-12-22', 'Люблю музыку', 800),
    (14, 34, '2023-12-23', 'Без громкой музыки', 950),
    (15, 35, '2023-12-24', 'Люблю поговорить', 1100),
    (16, 36, '2023-12-25', 'Без громкой музыки', 550),
    (17, 37, '2023-12-26', 'Люблю музыку', 700),
    (18, 38, '2023-12-27', 'Без разговоров', 850),
    (19, 39, '2023-12-28', 'Не курить, без животных', 1000),
    (20, 40, '2023-12-29', 'Люблю поговорить', 1150);
