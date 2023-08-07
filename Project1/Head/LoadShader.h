#pragma once

#include<bgfx/bgfx.h>
#include<bx/readerwriter.h>
#include<bx/file.h>
#include<bx/allocator.h>

namespace Shader {
	bx::AllocatorI* getDefaultAllocator()
	{
		BX_PRAGMA_DIAGNOSTIC_PUSH();
		BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459); // warning C4459: declaration of 's_allocator' hides global declaration
		BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
		static bx::DefaultAllocator s_allocator;
		return &s_allocator;
		BX_PRAGMA_DIAGNOSTIC_POP();
	}

	bx::AllocatorI* g_allocator = getDefaultAllocator();
	typedef bx::StringT<&g_allocator> String;
	static String s_currentDir;

	class FileReader : public bx::FileReader
	{
		typedef bx::FileReader super;

	public:
		virtual bool open(const bx::FilePath& _filePath, bx::Error* _err) override
		{
			String filePath(s_currentDir);
			filePath.append(_filePath);
			return super::open(filePath.getPtr(), _err);
		}
	};

	bgfx::ShaderHandle loadshader(const char* path) {
		bx::FileReaderI* shaderreader = BX_NEW(g_allocator, FileReader);
		bx::open(shaderreader, path);
		uint32_t size = (uint32_t)bx::getSize(shaderreader);
		const bgfx::Memory* mem = bgfx::alloc(size + 1);
		bx::read(shaderreader, mem->data, size, bx::ErrorAssert{});
		bx::close(shaderreader);
		mem->data[mem->size - 1] = '\0';
		return bgfx::createShader(mem);
	}
}