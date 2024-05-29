#ifndef VERSION_H
#define VERSION_H

namespace Version
{
	const int MAJOR = 1;
	const int MINOR = 2;
	const int LOCAL_REVISION = 1;
    const int BUILD = 2705;

	inline const char* versionString()
	{
        return "1.2.1.2705";
	}

	inline const char* globalRevision()
	{
		return "";
	}
}

#endif // VERSION_H
