# Cisco NX-SDK (Deprecated)

> [!WARNING]
> **NX-SDK is deprecated. Do not use NX-SDK for new applications or new development.**
>
> NX-SDK remains supported in Cisco NX-OS Release 10.7(1), but its use is
> strongly discouraged. Existing NX-SDK applications should begin migrating
> now and complete their migration away from NX-SDK by Cisco NX-OS Release
> 10.7(2).
>
> In Cisco NX-OS Release 10.7(2), NX-SDK will continue to function on the
> switch in a limited capacity. However, the public NX-SDK toolkit and source
> code hosted in this Cisco DevNet repository will be removed.

## Deprecation timeline

| Cisco NX-OS release | NX-SDK status |
| --- | --- |
| 10.7(1) | NX-SDK remains supported, but its use is strongly discouraged. Do not begin new NX-SDK development. |
| 10.7(2) | NX-SDK continues to function on the switch in a limited capacity. Existing applications should have migrated away from NX-SDK. The public toolkit and source code in this repository will be removed from Cisco DevNet. |

## What users should do

### Existing NX-SDK users

- Stop new NX-SDK application development.
- Identify existing applications and their dependencies on NX-SDK.
- Develop and execute a plan to migrate those applications away from NX-SDK
  by Cisco NX-OS Release 10.7(2).
- Work with your Cisco account team or support channels to identify alternative
  solutions for your requirements.

### New users

- Do not adopt NX-SDK or begin NX-SDK application development.
- Work with your Cisco account team or support channels to devise a supported
  solution for your use case.

## Remote NX-SDK

Remote or off-box NX-SDK capability is no longer provided by this repository.
References in historical repository documentation to applications running
outside NX-OS, remote NX-SDK libraries, or remote deployment are obsolete.

## About NX-SDK

NX-SDK is a legacy toolkit for developing applications that integrate with
Cisco Nexus NX-OS. It provides APIs for capabilities such as custom CLI
commands, syslogs, event handling, high availability, route management, and
streaming telemetry.

This repository contains historical public API headers, C++ development
stubs and build tools, and sample applications written in C++, Python, and Go.
These materials are retained temporarily to assist existing users during
migration and should not be treated as a recommendation to begin new NX-SDK
development.

## Legacy documentation

The following documentation is provided for existing users during migration:

- [Historical NX-SDK versions and features](versions.md)
- [NX-SDK API reference](https://nx-sdk.readthedocs.io/en/latest/)
- [Sample applications](examples/README.md)
- [Legacy on-box deployment instructions](readmes/NXSDK_in_NXOS.md)

Some linked documentation describes older releases or capabilities that are
no longer available. Consult the applicable Cisco NX-OS release notes for
current support information.

## License

This project is licensed under the [Apache License 2.0](LICENSE).
