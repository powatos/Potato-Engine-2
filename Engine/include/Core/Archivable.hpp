/** @file Archivable.hpp */
#pragma once

#include <functional>
#include <string>
#include <unordered_map>

/**
 * @def ARCHIVE_STATIC(type)
 * @brief Registers classes to be archived as a static actor
 * @remark Call this macro at the top of source files for archivable classes
 * @param type Literal name of the class
 */
#define ARCHIVE_STATIC(type) \
static struct ___##type##_Register { \
    ___##type##_Register() { \
        ___Archive::_GetArchive()[#type] = []() -> Archivable* { return new type(); }; \
    } \
} ___##type##_register;

/**
 * @brief Abstract factory class inherited by all classes that should be archived
 * @remark Persistence is currently still under development
 * @details This class is an abstract wrapper to streamline saving and loading objects. \n <b> Objects to be saved
 * must use @ref ARCHIVE_STATIC() "ARCHIVE_STATIC" </b>
 * @note This class *must* be inherited from for object classes that should archive objects.
 * This class has no inherent functionality.
 */
class Archivable
{
public:
    Archivable() = default;
    virtual ~Archivable() = default;
};

using ___ArchiveType = std::unordered_map< std::string, std::function<Archivable*()> >;

namespace ___Archive {
    inline ___ArchiveType& _GetArchive(){
        static ___ArchiveType archive;
        return archive;
    }
}
