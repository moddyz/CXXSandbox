#include <string>

// Declaration.
std::string GetLibraryName();

namespace B
{
std::string FindLibrary()
{
    return GetLibraryName();
}
} // namespace B
