
// Mostly extracted from Qt 4.7.1 sources hacks for checking if current style uses WinVista painting engine

#if defined(Q_OS_WIN32) || defined(Q_OS_WINCE)

#include <QWindowsVistaStyle>
#include <QApplication>
#include "windows.h"

typedef bool (WINAPI *PtrIsAppThemed)();
typedef bool (WINAPI *PtrIsThemeActive)();
static PtrIsAppThemed pIsAppThemed = 0;
static PtrIsThemeActive pIsThemeActive = 0;
static bool use_xp;


class QSystemLibrary
{
public:
	explicit QSystemLibrary(const QString &libraryName)
	{
		m_libraryName = libraryName;
		m_handle = 0;
		m_didLoad = false;
	}

	explicit QSystemLibrary(const wchar_t *libraryName)
	{
		m_libraryName = QString::fromWCharArray(libraryName);
		m_handle = 0;
		m_didLoad = false;
	}

	bool load(bool onlySystemDirectory = true)
	{
		m_handle = load((const wchar_t *)m_libraryName.utf16(), onlySystemDirectory);
		m_didLoad = true;
		return (m_handle != 0);
	}

	bool isLoaded()
	{
		return (m_handle != 0);
	}

	void *resolve(const char *symbol)
	{
		if (!m_didLoad)
			load();
		if (!m_handle)
			return 0;
#ifdef Q_OS_WINCE
		return (void*)GetProcAddress(m_handle, (const wchar_t*)QString::fromLatin1(symbol).utf16());
#else
		return (void*)GetProcAddress(m_handle, symbol);
#endif
	}

	static void *resolve(const QString &libraryName, const char *symbol)
	{
		return QSystemLibrary(libraryName).resolve(symbol);
	}

	static Q_CORE_EXPORT HINSTANCE load(const wchar_t *lpFileName, bool onlySystemDirectory = true);
private:
	HINSTANCE m_handle;
	QString m_libraryName;
	bool m_didLoad;
};

bool QWindowsXPStylePrivate_resolveSymbols()
{
	static bool tried = false;
	if (!tried) {
		tried = true;
		QSystemLibrary themeLib(QLatin1String("uxtheme"));
		pIsAppThemed = (PtrIsAppThemed)themeLib.resolve("IsAppThemed");
		if (pIsAppThemed)
			pIsThemeActive = (PtrIsThemeActive)themeLib.resolve("IsThemeActive");
	}
	return pIsAppThemed != 0;
}

bool QWindowsVistaStylePrivate_useVista()
{
	return (use_xp &&
		(QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA &&
		QSysInfo::WindowsVersion < QSysInfo::WV_NT_based));
}

bool FilesDelegate_win_useVista(QStyle *style)
{
	return qobject_cast<QWindowsVistaStyle *>(style) && QWindowsVistaStylePrivate_useVista();
}

void FilesDelegate_win_PaletteChanged()
{
	use_xp = QWindowsXPStylePrivate_resolveSymbols() && pIsThemeActive() && (pIsAppThemed() || !QApplication::instance());
}

#endif