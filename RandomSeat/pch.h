﻿// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 如有需要可以激活VMProtect保护，但是注意启用下面的宏后，需要在编译后的执行文件中添加VMProtectSDK32.dll或VMProtectSDK64.dll
//#define SU

// 添加要在此处预编译的标头
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include <vector>
#include <string>
#include <direct.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#ifdef SU
#include "VMProtectSDK.h"
#endif

#endif //PCH_H
