# 阿里云凭证 C++ SDK 示例

[English](README.md) | 简体中文

本目录包含演示如何使用阿里云凭证 C++ SDK 的示例代码。

## 示例列表

| 示例 | 描述 |
|------|------|
| `credential_examples.cpp` | 所有凭证类型的综合示例 |

## 构建示例

### 前置条件

1. 首先构建并安装凭证库：
   ```bash
   cd credentials-cpp
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build .
   sudo cmake --install .
   ```

2. 构建示例：
   ```bash
   cd examples
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```

### 运行示例

```bash
./credential_examples
```

## 凭证类型说明

示例演示了所有凭证类型：

| # | 类型 | 描述 |
|---|------|------|
| 1 | 默认凭证链 | 自动按顺序尝试多个凭证提供者 |
| 2 | AccessKey | 静态 Access Key 凭证 |
| 3 | STS | 临时安全令牌服务凭证 |
| 4 | RamRoleArn | RAM 角色 ARN 凭证（自动刷新 STS） |
| 5 | OIDCRoleArn | OIDC 角色 ARN 凭证 |
| 6 | EcsRamRole | ECS RAM 角色凭证（用于 ECS 实例） |
| 7 | URLCredential | 从 URL 端点获取的凭证 |
| 8 | BearerToken | Bearer Token 凭证（用于 CCC） |
| 9 | RsaKeyPair | RSA 密钥对凭证 |
| 10 | CLI Profile | 阿里云 CLI 配置 |
| 11 | Profile | 凭证文件（~/.alibabacloud/credentials.ini） |

## 环境变量

| 变量 | 描述 |
|------|------|
| `ALIBABA_CLOUD_ACCESS_KEY_ID` | Access Key ID |
| `ALIBABA_CLOUD_ACCESS_KEY_SECRET` | Access Key Secret |
| `ALIBABA_CLOUD_SECURITY_TOKEN` | Security Token（用于 STS） |
| `ALIBABA_CLOUD_ROLE_ARN` | 角色 ARN（用于 OIDC/RamRoleArn） |
| `ALIBABA_CLOUD_OIDC_PROVIDER_ARN` | OIDC 提供者 ARN |
| `ALIBABA_CLOUD_OIDC_TOKEN_FILE` | OIDC Token 文件路径 |
| `ALIBABA_CLOUD_ROLE_SESSION_NAME` | 角色会话名称 |
| `ALIBABA_CLOUD_ECS_METADATA` | ECS 角色名称 |
| `ALIBABA_CLOUD_CREDENTIALS_URI` | 凭证 URL |
| `ALIBABA_CLOUD_CLI_PROFILE_DISABLED` | 禁用 CLI Profile（true/false） |

## 配置文件

### CLI Profile (~/.aliyun/config.json)

通过 `aliyun configure` 命令创建。

### 凭证文件 (~/.alibabacloud/credentials.ini)

```ini
[default]
type = access_key
access_key_id = <your-access-key-id>
access_key_secret = <your-access-key-secret>

[ram_role_arn_profile]
type = ram_role_arn
access_key_id = <your-access-key-id>
access_key_secret = <your-access-key-secret>
role_arn = <your-role-arn>
role_session_name = <session-name>

[ecs_ram_role_profile]
type = ecs_ram_role
role_name = <your-ecs-role-name>

[oidc_profile]
type = oidc_role_arn
role_arn = <your-role-arn>
oidc_provider_arn = <your-oidc-provider-arn>
oidc_token_file_path = <path-to-token-file>
role_session_name = <session-name>
```
