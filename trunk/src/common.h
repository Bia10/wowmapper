#pragma once

#define SAFE_DELETE(p) { delete p; p = NULL; }
