import random
from random import randint

def generate_apartments(num_records, filename):
    first_names_m = [
    'Алексей', 'Дмитрий', 'Сергей', 'Николай', 'Иван', 'Андрей', 'Владимир', 'Михаил',
    'Анатолий', 'Виктор', 'Станислав', 'Роман', 'Константин', 'Евгений', 'Павел', 
    'Григорий', 'Юрий', 'Фёдор', 'Денис', 'Максим', 'Арсений', 'Игорь', 'Виталий',
    'Тимур', 'Александр', 'Даниил', 'Матвей', 'Станислав', 'Олег', 'Ярослав'
    ]

    first_names_f = [
    'Анна', 'Мария', 'Ольга', 'Елена', 'Татьяна', 'Наталья', 'Ирина', 'Светлана',
    'Ксения', 'Юлия', 'Анастасия', 'Дарья', 'Виктория', 'Екатерина', 'Людмила',
    'Нина', 'Алёна', 'Тамара', 'София', 'Вероника', 'Полина', 'Маргарита',
    'Зинаида', 'Евгения', 'Лилия', 'Оксана', 'Галина'
    ]

    last_names = [
    'Иванов', 'Петров', 'Сидоров', 'Смирнов', 'Кузнецов', 'Попов',
    'Васильев', 'Новиков', 'Фёдоров', 'Морозов', 'Волков', 'Соколов',
    'Зайцев', 'Беляев', 'Громов', 'Ковалёв', 'Мельников', 
    'Сергеев', 'Давыдов', 'Тихонов', 'Фролов',
    'Лебедев', 'Баранов', 'Кириллов', 'Семёнов'
    ]

    middle_names = [
    'Алексеевич', 'Дмитриевич', 'Сергеевич', 'Иванович', 
    'Петрович', 'Анатольевич', 'Николаевич', 
    'Владимирович', 'Андреевич', 
    'Михайлович', 'Викторович', 
    'Станиславович', 
    'Романович', 
    'Константинович',
    'Евгеньевич',
    'Павлович'
    ]

    middle_names_f = [
    'Алексеевна', 'Дмитриевна', 
    'Сергеевна', 
    'Ивановна',
    'Петровна',
    'Анатольевна',
    'Николаевна',
    'Владимировна',
    'Андреевна',
    'Михайловна',
    'Викторовна',
    'Станиславовна',
    'Романовна',
    'Константиновна',
    'Евгеньевна',
    'Павловна'
    ]

    with open(filename, 'w', encoding='utf-8') as f:
        for _ in range(num_records):
            apartment = randint(1, 20)
            area = randint(5, 300)
            rooms = randint(1, 5)
            price = round(random.uniform(20.0, 150.0), 1)
            floor = randint(1, 6)
            
            # Randomly choose gender for name
            if random.random() > 0.5:
                first_name = random.choice(first_names_m)
                middle_name = random.choice(middle_names)
                gender = 'm'
            else:
                first_name = random.choice(first_names_f)
                middle_name = random.choice(middle_names_f)
                gender = 'f'
            
            last_name = random.choice(last_names)
            if gender == 'f' and last_name[-1] not in ['а', 'я']:
                last_name += 'а'
            
            full_name = f"{last_name} {first_name} {middle_name}"
            
            line = f"{apartment},{area},{rooms},{price},{full_name},{floor}\n"
            f.write(line)

# Generate all files
sizes = [1000, 5000, 10000, 50000, 100000, 200000, 250000, 300000, 400000, 500000]
for size in sizes:
    generate_apartments(size, f"apartments_{size}.txt")