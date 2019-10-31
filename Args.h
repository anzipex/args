/** 
 * @file Args.h
 * @brief Описание класса Args
 * @author a.akulin
 * @date September 11, 2019
 */

#ifndef ARGS_H
#define ARGS_H

#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <sstream>
#include "ArgumentMarshaler.h"
#include "BooleanArgumentMarshaler.h"
#include "StringArgumentMarshaler.h"
#include "IntegerArgumentMarshaler.h"
#include "FloatArgumentMarshaler.h"
#include "DoubleArgumentMarshaler.h"
#include "StringArrayArgumentMarshaler.h"
#include "IntegerArrayArgumentMarshaler.h"
#include "FloatArrayArgumentMarshaler.h"

class Args final {
public:
    /**
     * @brief Принимает параметры форматной строки и набор аргументов для парсинга
     * @param schema, форматная строка.
     * 1) Необходимо указать имя ключа (только одна буква)
     * 2) Необходимо указать символ типа:
     *    без символа - Boolean
     *    * - String
     *    # - Integer
     *    #% - Float
     *    ## - Double
     *    [*] - вектор String
     *    [#] - вектор Integer
     *    [#%] - вектор Float
     * 3) ! после имени типа - ключ обязан быть в командной строке
     * @param argc, количество аргументов
     * @param argv, аргументы
     */
    Args(std::string schema, int argc, char** argv);
    ~Args() = default;

    /**
     * @brief Проверить ключ командной строки на соответствие указанному типу данных
     * @param arg, ключ командной строки
     * @return Результат проверки
     */
    bool check(char arg) const;

    /**
     * @brief Получить значение bool по соответствующему имени ключа
     * @param arg, ключ форматной строки
     * @return Значение
     */
    bool getBoolean(char arg);

    /**
     * @brief Получить значение string по соответствующему имени ключа
     * @param arg, ключ форматной строки
     * @return Значение
     */
    std::string getString(char arg);

    /**
     * @brief Получить значение integer по соответствующему имени ключа
     * @param arg, ключ форматной строки
     * @return Значение
     */
    int getInt(char arg);

    /**
     * @brief Получить значение float по соответствующему имени ключа
     * @param arg, ключ форматной строки
     * @return Значение
     */
    float getFloat(char arg);

    /**
     * @brief Получить значение double по соответствующему имени ключа
     * @param arg, ключ форматной строки
     * @return Значение 
     */
    double getDouble(char arg);

    /**
     * @brief Получить значение вектора string по соответствующему имени ключа
     * @param arg, ключ форматной строки
     * @return Значение
     */
    std::vector<std::string> getStringArray(char arg);

    /**
     * @brief Получить значение вектора integer по соответствующему имени ключа
     * @param arg, ключ форматной строки
     * @return Значение
     */
    std::vector<int> getIntArray(char arg);

    /**
     * @brief Получить значение вектора float по соответствующему имени ключа
     * @param arg, ключ форматной строки
     * @return Значение
     */
    std::vector<float> getFloatArray(char arg);

    /**
     * @brief Проверить, что все параметры распарсились без ошибок
     * @return результат проверки
     */
    bool isValid() const;

private:
    /**
     * @brief Получает вектор по каждому аргументу
     * @param argc, количество аргументов
     * @param argv, аргументы
     */
    void sequenceArgs(int argc, char** argv);

    /**
     * @brief Парсинг форматной строки
     * @param schema, форматная строка
     */
    void parseSchema(std::string schema);

    /**
     * @brief Разделить форматную строку по соответствующему символу
     * @param schema, форматная строка
     * @param delimiter, разделитель
     * @return Ключи и их значения
     */
    static std::vector<std::string> split(const std::string &schema, char delimiter);

    /**
     * @brief Парсинг каждого элемента форматной строки
     * @param element
     */
    void parseSchemaElement(std::string element);

    /**
     * @brief Проверка, является ли аргумент ключом
     * @param arg
     * @return Результат проверки
     */
    static bool isKey(std::string arg);

    /**
     * @brief Проверка, соответствия имени ключа
     * @param elementId, имя ключа
     */
    static void validateSchemaElementId(char elementId);

    /**
     * @brief Парсинг каждого аргумента командной строки
     */
    void parseArgStrings();

    /**
     * @brief Проверка, является ли следующий аргумент последним
     */
    bool nextNotKey();

    /**
     * @brief Проверка, является ли аргумент ключом bool
     */
    bool keyIsBool();

    /**
     * @brief Задать текущий аргумент
     */
    void setCurrentArg();

    /**
     * @brief Проверка наличия обязательных ключей и их значений
     * @return Наличие обязательных ключей и их значений
     */
    bool checkRequirements();

    /**
     * @brief Проверка валидностей всех аргументов
     */
    void checkValidity();

    /**
     * @brief Сделать вектор с обязательными ключами, если таковые имеются
     */
    void makeRequiredSchemaKeys();

    /**
     * @brief Проверка наличия обязательных ключей в форматной строке
     * @return Наличие хотя бы одного ключа
     */
    bool checkRequiredSchemaKeys() const;

    /**
     * @brief Проверка наличия значения у обязательного ключа среди аргументов командной строки
     * @param schemaKey, обязательный ключ форматной строки
     * @return Наличие значения у ключа из командной строки
     */
    bool checkRequiredKeysValue(std::string schemaKey);

    /**
     * @brief Проверка наличия ключей форматной и командной строки
     * @return Наличие обязательного ключа в командной строке
     */
    bool checkRequiredArgsKeys();

    char _argChar; //!< имя ключа
    bool _valid; //!< валидность
    std::map<char, std::unique_ptr<ArgumentMarshaler> > _marshalers; //!< имена ключей и значения
    std::map<char, std::string> _schemaArgs; //!< форматная строка из имен ключей и типов данных
    std::vector<std::string>::iterator _curArg; //!< текущий аргумент
    std::vector<std::string> _args; //!< аргументы командной строки
    std::vector<std::string> _requiredShemaKeys; //!< обязательные ключи в форматной строке
    std::vector<std::string> _requiredArgsKeys; //!< обязательные ключи в командной строке
    std::vector<bool> _validity; //!< валидность всех аргументов
};

#endif /* ARGS_H */