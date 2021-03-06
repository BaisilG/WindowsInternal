#include "stdafx.h"

using namespace std;

static const size_t cell_size = 256;

void read(void *pBlock, int cell)
{
	__try {
		wcout << static_cast<wchar_t *>(pBlock) + cell * cell_size << endl;
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ?
		EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
		wcout << L"There is no data at cell No. " << cell << endl;
	}
}

bool write(void *pBlock, int cell)
{
	wchar_t *p = static_cast<wchar_t *>(pBlock) + cell * cell_size;
	bool done = false;
	__try {
		const size_t bufferCount = cell_size / sizeof(wchar_t);
		swprintf_s(p, bufferCount, L"Data written to cell %d...\n", cell);
		done = true;
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ?
		EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
		VirtualAlloc(p, cell_size, MEM_COMMIT, PAGE_READWRITE);
	}
	return done;
}

int main()
{
	void *pBlock = VirtualAlloc(nullptr, cell_size * (1 << 20), MEM_RESERVE, PAGE_READWRITE);
	wcout << L"Block address: " << pBlock << endl;

	do {
		wcout << L"Enter cell number: ";
		int cell;
		wcin >> cell;
		if (cell < 0) break;

		wcout << L"1=read 2=write: ";
		int option;
		wcin >> option;

		// read
		if (option == 1) {
			read(pBlock, cell);
			continue;
		}

		// write
		bool done = false;
		while (!done) {
			done = write(pBlock, cell);
		}
	} while (true);

	VirtualFree(pBlock, 0, MEM_RELEASE);

	return 0;
}
