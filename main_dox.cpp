/**
 * @file main.cpp
 * @brief Сравнение эффективности структур данных для поиска по ключу.
 *
 * Программа загружает данные из файлов определённых размеров и измеряет
 * время поиска по ключу (ФИО) в различных структурах данных:
 * - std::vector (линейный поиск)
 * - BST (двоичное дерево поиска)
 * - RBT (красно-чёрное дерево)
 * - HashTable (хеш с открытой адресацией)
 * - std::multimap (STL)
 *
 * Результаты заносятся в CSV-файл для анализа.
 */

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <chrono>
#include <fstream>
#include <random>
#include <algorithm>
#include <sstream>

/**
 * @brief Структура хранения одного элемента данных.
 */
struct Data {
    std::string key; ///< Ключ, напр. ФИО
    int value;       ///< Некоторое числовое значение
};

/**
 * @brief Линейный поиск элемента по ключу.
 *
 * @param data Контейнер элементов
 * @param key Искомый ключ
 * @return Вектор всех найденных записей
 */
std::vector<Data> linearSearch(const std::vector<Data>& data, const std::string& key) {
    std::vector<Data> results;
    for (const auto& item : data) {
        if (item.key == key)
            results.push_back(item);
    }
    return results;
}

/**
 * @brief Узел для двоичного дерева поиска.
 */
struct BSTNode {
    Data data;
    BSTNode* left = nullptr;  ///< Указатель на левое поддерево
    BSTNode* right = nullptr; ///< Указатель на правое поддерево
};

/**
 * @brief Вставка элемента в BST.
 *
 * @param root Указатель на корень дерева
 * @param value Вставляемый элемент
 */
void insertBST(BSTNode*& root, const Data& value) {
    if (!root) {
        root = new BSTNode{value};
    } else if (value.key < root->data.key) {
        insertBST(root->left, value);
    } else {
        insertBST(root->right, value);
    }
}

/**
 * @brief Поиск всех элементов с заданным ключом в BST.
 *
 * @param root Корень дерева
 * @param key Искомый ключ
 * @param results Вектор для сохранения найденных записей
 */
void searchBST(BSTNode* root, const std::string& key, std::vector<Data>& results) {
    if (!root) return;
    if (root->data.key == key)
        results.push_back(root->data);
    if (key <= root->data.key)
        searchBST(root->left, key, results);
    if (key >= root->data.key)
        searchBST(root->right, key, results);
}

/**
 * @brief Цвет узла в красно-чёрном дереве.
 */
enum Color { RED, BLACK };

/**
 * @brief Узел красно-чёрного дерева.
 */
struct RBNode {
    std::string key;
    std::vector<Data> values;
    Color color;
    RBNode* parent = nullptr;
    RBNode* left = nullptr;
    RBNode* right = nullptr;

    RBNode(const std::string& k, const Data& d)
        : key(k), values{d}, color(RED) {}
};

static RBNode* rbRoot = nullptr; ///< Глобальный корень RBT

// Вспомогательные функции поворотов и балансировки RBT

void leftRotate(RBNode*& root, RBNode* x) {
    if (!x || !x->right) return;
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void rightRotate(RBNode*& root, RBNode* x) {
    if (!x || !x->left) return;
    RBNode* y = x->left;
    x->left = y->right;
    if (y->right) y->right->parent = x;
    y->parent = x->parent;
    if (!x->parent) root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
}

void fixInsert(RBNode*& root, RBNode* node) {
    while (node != root && node->parent && node->parent->color == RED) {
        RBNode* parent = node->parent;
        RBNode* grand = parent->parent;
        if (!grand) break;

        RBNode* uncle = (parent == grand->left) ? grand->right : grand->left;
        if (uncle && uncle->color == RED) {
            parent->color = uncle->color = BLACK;
            grand->color = RED;
            node = grand;
        } else {
            if ((parent == grand->left && node == parent->right)) {
                leftRotate(root, parent);
                node = parent;
                parent = node->parent;
            } else if ((parent == grand->right && node == parent->left)) {
                rightRotate(root, parent);
                node = parent;
                parent = node->parent;
            }
            parent->color = BLACK;
            grand->color = RED;
            if (parent == grand->left) rightRotate(root, grand);
            else leftRotate(root, grand);
        }
    }
    if (root) root->color = BLACK;
}

/**
 * @brief Вставка элемента в красно-чёрное дерево.
 *
 * @param root Ссылка на корень дерева
 * @param d Вставляемый элемент
 */
void insertRBT(RBNode*& root, const Data& d) {
    RBNode* node = root;
    RBNode* parent = nullptr;
    while (node) {
        parent = node;
        if (d.key == node->key) {
            node->values.push_back(d);
            return;
        } else if (d.key < node->key)
            node = node->left;
        else
            node = node->right;
    }
    RBNode* newNode = new RBNode(d.key, d);
    newNode->parent = parent;
    if (!parent) root = newNode;
    else if (d.key < parent->key) parent->left = newNode;
    else parent->right = newNode;
    fixInsert(root, newNode);
}

/**
 * @brief Поиск всех элементов по ключу в красно-чёрном дереве.
 *
 * @param root Корень дерева
 * @param key Искомый ключ
 * @return Вектор найденных элементов (пустой, если не найдено)
 */
std::vector<Data> searchRBT(RBNode* root, const std::string& key) {
    RBNode* node = root;
    while (node) {
        if (key == node->key)
            return node->values;
        else if (key < node->key)
            node = node->left;
        else
            node = node->right;
    }
    return {};
}

/**
 * @brief Элемент хеш-таблицы.
 */
struct HashEntry {
    bool occupied = false; ///< Флаг занятости
    Data data;             ///< Хранимые данные
};

/**
 * @brief Хеш-таблица с открытой адресацией.
 */
struct HashTable {
    std::vector<HashEntry> table; ///< Контейнер
    size_t size;                  ///< Размер таблицы
    int collisions = 0;          ///< Счётчик коллизий

    /**
     * @brief Конструктор.
     *
     * @param s Размер таблицы
     */
    HashTable(size_t s) : size(s), table(s) {}

    /**
     * @brief Хеш-функция на основе djb2.
     *
     * @param key Строковый ключ
     * @return Индекс в таблице
     */
    size_t hash(const std::string& key) {
        unsigned long h = 5381;
        for (char c : key)
            h = ((h << 5) + h) + static_cast<unsigned char>(c);
        return h % size;
    }

    /**
     * @brief Вставка элемента в хеш-таблицу.
     *
     * @param d Вставляемый элемент
     */
    void insert(const Data& d) {
        size_t idx = hash(d.key);
        size_t original = idx;
        int steps = 0;
        while (table[idx].occupied) {
            if (table[idx].data.key == d.key) break;
            idx = (idx + 1) % size;
            steps++;
            if (idx == original) return;
        }
        if (steps > 0) collisions++;
        table[idx].occupied = true;
        table[idx].data = d;
    }

    /**
     * @brief Поиск элемента по ключу.
     *
     * @param key Искомый ключ
     * @return Вектор с найденным элементом или пустой
     */
    std::vector<Data> search(const std::string& key) {
        size_t idx = hash(key);
        size_t original = idx;
        while (table[idx].occupied) {
            if (table[idx].data.key == key)
                return {table[idx].data};
            idx = (idx + 1) % size;
            if (idx == original) break;
        }
        return {};
    }
};

/**
 * @brief Загружает данные из CSV-файла.
 *
 * @param filename Путь к файлу
 * @return Вектор структур Data
 */
std::vector<Data> loadDataset(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Data> result;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, ',')) tokens.push_back(token);
        if (tokens.size() >= 5) {
            Data d;
            d.key = tokens[4];
            d.value = rand();
            result.push_back(d);
        }
    }
    return result;
}

/**
 * @brief Замеряет время выполнения функции.
 *
 * @tparam Func Тип функции/лямбда без параметров
 * @param f Объект-функция для вызова
 * @return Время работы в микросекундах
 */
template<typename Func>
long long measureTime(Func f) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

/**
 * @brief Основная функция программы.
 *
 * Последовательно для каждого объема данных выполняется:
 * 1) загрузка;
 * 2) построение структур;
 * 3) 10 рандомных поисков;
 * 4) замер времени;
 * 5) подсчёт коллизий;
 * 6) запись результатов в CSV.
 *
 * @return Код выхода (0 — успех, !=0 — ошибка)
 */
int main() {
    std::ofstream out("results.csv");
    out << "Size,Linear,BST,RBT,Hash,Multimap,Collisions\n";

    std::vector<int> sizes = {1000, 5000, 10000, 50000,
                              100000, 200000, 250000,
                              300000, 400000, 500000};
    int repeats = 10;
    for (int size : sizes) {
        std::string filename = "apartments_" + std::to_string(size) + ".txt";
        auto data = loadDataset(filename);
        if (data.empty()) {
            std::cerr << "[ERROR] Cannot load " << filename << "\n";
            continue;
        }

        long long totalLinear = 0, totalBST = 0, totalRBT = 0;
        long long totalHash = 0, totalMM = 0;
        int totalCollisions = 0;

        for (int rep = 0; rep < repeats; ++rep) {
            std::string targetKey = data[rand() % data.size()].key;

            totalLinear += measureTime([&]() {
                linearSearch(data, targetKey);
            });

            BSTNode* bstRoot = nullptr;
            for (auto& d : data) insertBST(bstRoot, d);
            totalBST += measureTime([&]() {
                std::vector<Data> res; searchBST(bstRoot, targetKey, res);
            });

            rbRoot = nullptr;
            for (auto& d : data) insertRBT(rbRoot, d);
            totalRBT += measureTime([&]() {
                searchRBT(rbRoot, targetKey);
            });

            HashTable ht(size * 2);
            for (auto& d : data) ht.insert(d);
            totalCollisions += ht.collisions;
            totalHash += measureTime([&]() {
                ht.search(targetKey);
            });

            std::multimap<std::string, Data> mm;
            for (auto& d : data) mm.insert({d.key, d});
            totalMM += measureTime([&]() {
                auto rng = mm.equal_range(targetKey);
                for (auto it = rng.first; it != rng.second; ++it) {}
            });
        }

        out << size << ","
            << totalLinear / repeats << ","
            << totalBST / repeats << ","
            << totalRBT / repeats << ","
            << totalHash / repeats << ","
            << totalMM / repeats << ","
            << (totalCollisions / repeats) << "\n";

        std::cout << "Size: " << size << " done.\n";
    }
    return 0;
}
