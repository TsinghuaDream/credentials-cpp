# Alibaba Cloud Credentials C++ SDK Examples

[English](README.md) | [简体中文](README-zh-CN.md)

This directory contains example code demonstrating how to use the Alibaba Cloud Credentials C++ SDK.

## Examples

| Example | Description |
|---------|-------------|
| `credential_examples.cpp` | Comprehensive examples for all credential types |

## Building the Examples

### Prerequisites

1. Build and install the credentials library first:
   ```bash
   cd credentials-cpp
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build .
   sudo cmake --install .
   ```

2. Build the examples:
   ```bash
   cd examples
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```

### Running Examples

```bash
./credential_examples
```

## Credential Types Covered

The example demonstrates all credential types:

| # | Type | Description |
|---|------|-------------|
| 1 | Default Chain | Automatically tries multiple providers in order |
| 2 | AccessKey | Static Access Key credentials |
| 3 | STS | Temporary Security Token Service credentials |
| 4 | RamRoleArn | RAM Role ARN credentials (auto-refresh STS) |
| 5 | OIDCRoleArn | OIDC Role ARN credentials |
| 6 | EcsRamRole | ECS RAM Role credentials (for ECS instances) |
| 7 | URLCredential | Credentials from URL endpoint |
| 8 | BearerToken | Bearer Token credentials (for CCC) |
| 9 | RsaKeyPair | RSA Key Pair credentials |
| 10 | CLI Profile | Alibaba Cloud CLI configuration |
| 11 | Profile | Credentials file (~/.alibabacloud/credentials.ini) |

## Environment Variables

| Variable | Description |
|----------|-------------|
| `ALIBABA_CLOUD_ACCESS_KEY_ID` | Access Key ID |
| `ALIBABA_CLOUD_ACCESS_KEY_SECRET` | Access Key Secret |
| `ALIBABA_CLOUD_SECURITY_TOKEN` | Security Token (for STS) |
| `ALIBABA_CLOUD_ROLE_ARN` | Role ARN (for OIDC/RamRoleArn) |
| `ALIBABA_CLOUD_OIDC_PROVIDER_ARN` | OIDC Provider ARN |
| `ALIBABA_CLOUD_OIDC_TOKEN_FILE` | OIDC Token File Path |
| `ALIBABA_CLOUD_ROLE_SESSION_NAME` | Role Session Name |
| `ALIBABA_CLOUD_ECS_METADATA` | ECS Role Name |
| `ALIBABA_CLOUD_CREDENTIALS_URI` | Credentials URL |
| `ALIBABA_CLOUD_CLI_PROFILE_DISABLED` | Disable CLI Profile (true/false) |

## Configuration Files

### CLI Profile (~/.aliyun/config.json)

Created by `aliyun configure` command.

### Credentials File (~/.alibabacloud/credentials.ini)

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
