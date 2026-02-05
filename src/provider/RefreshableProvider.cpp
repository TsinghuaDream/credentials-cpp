#include <alibabacloud/credentials/provider/RefreshableProvider.hpp>

namespace AlibabaCloud {
namespace Credentials {

// C++11 requires out-of-class definition for constexpr static members
// even if they have in-class initializers
constexpr int64_t RefreshableProvider::STALE_TIME_WINDOW;
constexpr int64_t RefreshableProvider::PREFETCH_THRESHOLD;
constexpr int64_t RefreshableProvider::REFRESH_BLOCKING_MAX_WAIT_MS;

} // namespace Credentials
} // namespace AlibabaCloud
