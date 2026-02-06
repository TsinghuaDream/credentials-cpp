#include <alibabacloud/credentials/provider/RefreshableProvider.hpp>

namespace AlibabaCloud {
namespace Credentials {

// Note: In C++17+, inline variables make class-external definitions redundant.
// However, for C++11/14 compatibility, we need these definitions when the constant
// is ODR-used (e.g., as a function parameter).
#if __cplusplus < 201703L
constexpr int64_t RefreshableProvider::STALE_TIME_WINDOW;
constexpr int64_t RefreshableProvider::PREFETCH_THRESHOLD;
constexpr int64_t RefreshableProvider::REFRESH_BLOCKING_MAX_WAIT_MS;
#endif

} // namespace Credentials
} // namespace AlibabaCloud
