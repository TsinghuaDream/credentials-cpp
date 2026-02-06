[English](./README.md) | 简体中文

![Alibaba Cloud Logo](https://aliyunsdk-pages.alicdn.com/icons/AlibabaCloud.svg)

# Alibaba Cloud Credentials for C++

[![codecov](https://codecov.io/gh/aliyun/credentials-cpp/branch/master/graph/badge.svg)](https://codecov.io/gh/aliyun/credentials-cpp)
[![Travis Build Status](https://travis-ci.org/aliyun/credentials-cpp.svg?branch=master)](https://travis-ci.org/aliyun/credentials-cpp)

Alibaba Cloud Credentials for C++ 是帮助 C++ 开发者管理凭据的工具。

本文将介绍如何获取和使用 Credentials for C++。

## 环境要求

### 编译器要求
- **Windows**: Visual Studio 2015 或以上版本
- **Linux**: GCC 4.9 或以上版本  
- **macOS**: Clang (Xcode Command Line Tools)

### 构建工具
- **CMake**: 3.13 或以上版本（Windows 推荐 3.15+）
- **C++ 标准**: C++11 或更高版本

### 系统要求
- **内存**: 建议 4GB 或以上
- **磁盘空间**: 建议至少 500MB 可用空间

### 依赖库

本库依赖 **darabonba-core**，需要安装以下系统库：

- **OpenSSL**: 用于加密和网络通信
- **libcurl**: 用于 HTTP 请求
- **zlib**: 用于压缩（curl 依赖，静态构建时必需）
- **uuid** (仅 Linux): 用于生成 UUID

**快速安装所有依赖：**

```bash
# Ubuntu/Debian
sudo apt-get install -y libssl-dev libcurl4-openssl-dev zlib1g-dev uuid-dev

# CentOS/RHEL
sudo yum install -y openssl-devel libcurl-devel zlib-devel libuuid-devel

# macOS (Homebrew)
brew install openssl curl

# Windows (vcpkg)
vcpkg install openssl curl zlib
```

## 安装

### Linux/macOS

```bash
git clone https://github.com/aliyun/credentials-cpp.git
cd credentials-cpp
sh scripts/install.sh
```

### Windows

1. 使用 git-bash 执行以下命令，从 GitHub 克隆源码：

```bash
git clone https://github.com/aliyun/credentials-cpp.git
```

2. 使用 Visual Studio 进行编译：
   - 在根目录下创建 `cmake_build` 目录
   - 打开 CMake GUI，然后进行以下操作：
     - 选择 `Browse Source` 为源代码目录（`credentials-cpp`）
     - 选择 `Browse Build` 为构建目录（`cmake_build`）
     - 点击 `Configure`
     - 点击 `Generate`，生成 VS 解决方案

3. 编译安装 C++ SDK：
   - 进入 `cmake_build` 目录，使用 Visual Studio 打开 `alibabacloud_credentials.sln` 解决方案
   - 选择 `Release` 配置
   - 在"生成"->"配置管理器"中勾选 INSTALL
   - 生成 -> 生成解决方案

### 命令行构建（跨平台）

```bash
# 配置项目
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# 构建
cmake --build build --config Release

# 安装（Linux/macOS 可能需要 sudo）
cmake --install build
```

**构建选项：**

| 选项 | 默认值 | 说明 |
|------|--------|------|
| `BUILD_SHARED_LIBS` | ON | 构建共享库 |
| `ENABLE_UNIT_TESTS` | OFF | 启用单元测试 |

## 快速使用

在您开始之前，您需要注册阿里云帐户并获取您的[凭证](https://usercenter.console.aliyun.com/#/manage/ak)。

### 使用默认凭证提供者链（推荐）

```cpp
#include <alibabacloud/credentials/Client.hpp>
#include <iostream>

using namespace AlibabaCloud::Credentials;

int main() {
    // 使用默认凭证提供者链
    // 自动从环境变量、配置文件等处获取凭证
    Client client;
    
    // 获取凭证
    auto credential = client.getCredential();
    std::cout << "AccessKeyId: " << credential.getAccessKeyId() << std::endl;
    std::cout << "Type: " << credential.getType() << std::endl;
    
    return 0;
}
```

**安全提示：** 请勿在代码中硬编码 AccessKey ID 和 AccessKey Secret，推荐使用环境变量或配置文件。

### 凭证类型

#### AccessKey

通过[用户信息管理][ak]设置 access_key，它们具有该账户完全的权限，请妥善保管。有时出于安全考虑，您不能把具有完全访问权限的主账户 AccessKey 交于一个项目的开发者使用，您可以[创建RAM子账户][ram]并为子账户[授权][permissions]，使用RAM子用户的 AccessKey 来进行API调用。

**⚠️ 安全警告：请勿在代码中硬编码 AccessKey！**

**推荐方式：通过环境变量获取**

```cpp
#include <alibabacloud/credentials/Client.hpp>
#include <cstdlib>  // for std::getenv

using namespace AlibabaCloud::Credentials;

int main() {
    // 从环境变量获取凭证
    const char* accessKeyId = std::getenv("ALIBABA_CLOUD_ACCESS_KEY_ID");
    const char* accessKeySecret = std::getenv("ALIBABA_CLOUD_ACCESS_KEY_SECRET");
    
    if (!accessKeyId || !accessKeySecret) {
        // 处理凭证未设置的情况
        return 1;
    }
    
    Models::Config config;
    config.setType("access_key")
          .setAccessKeyId(accessKeyId)
          .setAccessKeySecret(accessKeySecret);
    
    Client client(config);
    auto credential = client.getCredential();
    
    return 0;
}
```

设置环境变量：

```bash
# Linux/macOS
export ALIBABA_CLOUD_ACCESS_KEY_ID="<your-access-key-id>"
export ALIBABA_CLOUD_ACCESS_KEY_SECRET="<your-access-key-secret>"

# Windows (PowerShell)
$env:ALIBABA_CLOUD_ACCESS_KEY_ID="<your-access-key-id>"
$env:ALIBABA_CLOUD_ACCESS_KEY_SECRET="<your-access-key-secret>"

# Windows (CMD)
set ALIBABA_CLOUD_ACCESS_KEY_ID=<your-access-key-id>
set ALIBABA_CLOUD_ACCESS_KEY_SECRET=<your-access-key-secret>
```

#### STS

通过安全令牌服务（Security Token Service，简称 STS），申请临时安全凭证（Temporary Security Credentials，简称 TSC），创建临时安全凭证。

**注意：STS 凭证通常从 STS 服务获取，请勿硬编码！**

```cpp
#include <alibabacloud/credentials/Client.hpp>
#include <cstdlib>

using namespace AlibabaCloud::Credentials;

int main() {
    // 从环境变量获取 STS 凭证
    const char* accessKeyId = std::getenv("ALIBABA_CLOUD_ACCESS_KEY_ID");
    const char* accessKeySecret = std::getenv("ALIBABA_CLOUD_ACCESS_KEY_SECRET");
    const char* securityToken = std::getenv("ALIBABA_CLOUD_SECURITY_TOKEN");
    
    if (!accessKeyId || !accessKeySecret || !securityToken) {
        return 1;
    }
    
    Models::Config config;
    config.setType("sts")
          .setAccessKeyId(accessKeyId)
          .setAccessKeySecret(accessKeySecret)
          .setSecurityToken(securityToken);
    
    Client client(config);
    
    return 0;
}
```

#### RamRoleArn

通过指定[RAM角色][RAM Role]，让凭证自动申请维护 STS Token。你可以通过为 `Policy` 赋值来限制获取到的 STS Token 的权限。

**注意：初始 AccessKey 应从环境变量获取！**

```cpp
#include <alibabacloud/credentials/Client.hpp>
#include <cstdlib>

using namespace AlibabaCloud::Credentials;

int main() {
    // 从环境变量获取初始凭证
    const char* accessKeyId = std::getenv("ALIBABA_CLOUD_ACCESS_KEY_ID");
    const char* accessKeySecret = std::getenv("ALIBABA_CLOUD_ACCESS_KEY_SECRET");
    
    if (!accessKeyId || !accessKeySecret) {
        return 1;
    }
    
    Models::Config config;
    config.setType("ram_role_arn")
          .setAccessKeyId(accessKeyId)
          .setAccessKeySecret(accessKeySecret)
          .setRoleArn("<your-role-arn>")  // 或从环境变量 ALIBABA_CLOUD_ROLE_ARN 获取
          .setRoleSessionName("<session-name>")
          .setPolicy("<policy>")  // 可选
          .setExternalId("<external-id>")  // 可选
          .setRoleSessionExpiration(3600);  // 可选
    
    Client client(config);
    
    return 0;
}
```

#### OIDCRoleArn

通过指定[OIDC 角色][OIDC Role]，让凭证自动申请维护 STS Token。你可以通过为 `Policy` 赋值来限制获取到的 STS Token 的权限。

```cpp
#include <alibabacloud/credentials/Client.hpp>

using namespace AlibabaCloud::Credentials;

int main() {
    Models::Config config;
    config.setType("oidc_role_arn")
          .setRoleArn("RoleArn")
          .setOidcProviderArn("OIDCProviderArn")
          .setOidcTokenFilePath("/Users/xxx/xxx")
          .setRoleSessionName("RoleSessionName")
          .setPolicy("policy")  // 可选
          .setRoleSessionExpiration(3600);  // 可选
    
    Client client(config);
    
    return 0;
}
```

#### EcsRamRole

通过指定角色名称，让凭证自动申请维护 STS Token

```cpp
#include <alibabacloud/credentials/Client.hpp>

using namespace AlibabaCloud::Credentials;

int main() {
    Models::Config config;
    config.setType("ecs_ram_role")
          .setRoleName("RoleName")  // 可选，不填则自动获取
          .setDisableIMDSv1(true);  // 可选，禁用 V1 兜底
    
    Client client(config);
    
    return 0;
}
```

#### URLCredential

通过指定提供凭证的自定义网络服务地址，让凭证自动申请维护 STS Token

```cpp
#include <alibabacloud/credentials/Client.hpp>

using namespace AlibabaCloud::Credentials;

int main() {
    Models::Config config;
    config.setType("credentials_uri")
          .setCredentialsURL("http://xxx");
    
    Client client(config);
    
    return 0;
}
```

#### Bearer Token

如呼叫中心(CCC)需用此凭证，请自行申请维护 Bearer Token。

**注意：BearerToken 应从环境变量或安全服务获取！**

```cpp
#include <alibabacloud/credentials/Client.hpp>
#include <cstdlib>

using namespace AlibabaCloud::Credentials;

int main() {
    // 从环境变量或安全服务获取 BearerToken
    const char* bearerToken = std::getenv("ALIBABA_CLOUD_BEARER_TOKEN");
    
    if (!bearerToken) {
        return 1;
    }
    
    Models::Config config;
    config.setType("bearer")
          .setBearerToken(bearerToken);
    
    Client client(config);
    
    return 0;
}
```

### 使用默认凭证提供链

如果你调用 `Client client;` 时，将通过凭证提供链来为你获取凭证。

默认凭证提供程序链查找可用的凭证，寻找顺序如下：

1. **环境变量**
   在环境变量里寻找环境凭证，如果定义了 `ALIBABA_CLOUD_ACCESS_KEY_ID` 和 `ALIBABA_CLOUD_ACCESS_KEY_SECRET` 环境变量且不为空，程序将使用它们创建默认凭证。

2. **OIDC RAM 角色**
   通过环境变量 `ALIBABA_CLOUD_ROLE_ARN`、`ALIBABA_CLOUD_OIDC_PROVIDER_ARN` 和 `ALIBABA_CLOUD_OIDC_TOKEN_FILE` 配置。

3. **CLI 配置文件**
   如果用户主目录存在 CLI 配置文件 `~/.aliyun/config.json`，程序会自动创建指定类型和名称的凭证。

4. **配置文件**
   如果用户主目录存在默认文件 `~/.alibabacloud/credentials`，程序会自动创建指定类型和名称的凭证。

5. **ECS 实例 RAM 角色**
   通过 ECS 实例元数据服务（IMDS）获取（如果设置了 `ALIBABA_CLOUD_ECS_METADATA`）。

6. **Credentials URI**
   通过环境变量 `ALIBABA_CLOUD_CREDENTIALS_URI` 指定的 URL 获取。

**通过环境变量设置凭证：**

```bash
# Linux/macOS
export ALIBABA_CLOUD_ACCESS_KEY_ID="<your-access-key-id>"
export ALIBABA_CLOUD_ACCESS_KEY_SECRET="<your-access-key-secret>"

# Windows (PowerShell)
$env:ALIBABA_CLOUD_ACCESS_KEY_ID="<your-access-key-id>"
$env:ALIBABA_CLOUD_ACCESS_KEY_SECRET="<your-access-key-secret>"

# Windows (CMD)
set ALIBABA_CLOUD_ACCESS_KEY_ID=<your-access-key-id>
set ALIBABA_CLOUD_ACCESS_KEY_SECRET=<your-access-key-secret>
```

**通过配置文件设置凭证（`~/.alibabacloud/credentials`）：**

```ini
[default]                          # 默认配置
enable = true                      # 启用，没有该选项默认不启用
type = access_key                  # 认证方式为 access_key
access_key_id = foo                # Key
access_key_secret = bar            # Secret

[client1]                          # 命名为 `client1` 的配置
type = ecs_ram_role                # 认证方式为 ecs_ram_role
role_name = EcsRamRoleTest         # Role Name

[client2]                          # 命名为 `client2` 的配置
enable = false                     # 不启用
type = ram_role_arn                # 认证方式为 ram_role_arn
region_id = cn-test                # 获取session用的region
policy = test                      # 选填 指定权限
access_key_id = foo
access_key_secret = bar
role_arn = role_arn                # 可通过设置环境变量 ALIBABA_CLOUD_ROLE_ARN 来代替
role_session_name = session_name   # 选填

[client3]                          # 命名为 `client3` 的配置
enable = false                     # 不启用
type = oidc_role_arn               # 认证方式为 oidc_role_arn
region_id = cn-test                # 获取session用的region
policy = test                      # 选填 指定权限
role_arn = role_arn                # 可通过设置环境变量 ALIBABA_CLOUD_ROLE_ARN 来代替
oidc_provider_arn = oidc_provider_arn # 可通过设置环境变量 ALIBABA_CLOUD_OIDC_PROVIDER_ARN 来代替
oidc_token_file_path = /xxx/xxx    # 可通过设置环境变量 ALIBABA_CLOUD_OIDC_TOKEN_FILE 来代替
role_session_name = session_name   # 选填
```

## 问题

[提交 Issue](https://github.com/aliyun/credentials-cpp/issues/new)，不符合指南的问题可能会立即关闭。

## 发行说明

每个版本的详细更改记录在[发行说明](./CHANGELOG.md)中。

## 贡献

提交 Pull Request 之前请阅读[贡献指南](./.github/PULL_REQUEST_TEMPLATE.md)。

## 相关

* [OpenAPI 开发者门户](https://api.aliyun.com/)
* [诊断平台](https://api.aliyun.com/troubleshoot)

## 许可证

[Apache-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Copyright 2009-present Alibaba Cloud All rights reserved.

[ak]: https://usercenter.console.aliyun.com/#/manage/ak
[ram]: https://ram.console.aliyun.com/users
[permissions]: https://ram.console.aliyun.com/permissions
[RAM Role]: https://ram.console.aliyun.com/#/role/list
[OIDC Role]: https://help.aliyun.com/zh/ram/user-guide/role-based-sso-by-using-oidc
