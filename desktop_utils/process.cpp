#include "stdafx.h"

void Process::wait(HANDLE processHandle) {
  WaitForSingleObject(processHandle, INFINITE);
}
