#include <string>

// Declaration.
std::string GetLibraryName();

namespace A
{
std::string FindLibrary()
{
    return GetLibraryName();
}
} // namespace A
