#pragma once
#include "Event.h"
#include "Singleton.h"
#include "Shortcut.h"
#include <winrt/Magpie.h>

namespace Magpie {

struct Profile;

class ShortcutService : public Singleton<ShortcutService> {
	friend Singleton<ShortcutService>;

public:
	void Initialize();

	void Uninitialize() noexcept;

	bool IsError(winrt::Magpie::ShortcutAction action) const noexcept {
		return _shortcutInfos[(size_t)action].isError;
	}

	// profileIdx < 0 表示默认配置
	bool IsProfileShortcutError(int profileIdx, bool windowedMode) const noexcept;

	void StopKeyboardHook() noexcept {
		_isKeyboardHookActive = false;
	}

	void StartKeyboardHook() noexcept {
		_isKeyboardHookActive = true;
	}

	Event<winrt::Magpie::ShortcutAction> ShortcutActivated;
	// 参数: 配置文件索引（<0 表示默认配置）、是否为窗口模式缩放
	Event<int, bool> ProfileShortcutActivated;

private:
	ShortcutService() = default;

	static LRESULT _WndProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		return Get()._WndProc(hWnd, msg, wParam, lParam);
	}
	LRESULT _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void _AppSettings_OnShortcutChanged(winrt::Magpie::ShortcutAction action);

	void _RegisterShortcut(winrt::Magpie::ShortcutAction action);

	void _FireShortcut(winrt::Magpie::ShortcutAction action);

	void _RegisterProfileShortcuts();

	void _UnregisterProfileShortcuts() noexcept;

	void _FireProfileShortcut(size_t infoIdx);

	static LRESULT CALLBACK _LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

	struct _ShortcutInfo {
		std::chrono::steady_clock::time_point lastFireTime{};
		bool isError = true;
	};
	std::array<_ShortcutInfo, (size_t)winrt::Magpie::ShortcutAction::COUNT_OR_NONE> _shortcutInfos{};

	struct _ProfileShortcutInfo {
		Shortcut shortcut;
		std::chrono::steady_clock::time_point lastFireTime{};
		int profileIdx = -1;
		bool windowedMode = false;
		bool isError = true;
	};
	// 只包含非空的配置文件热键，热键 id 为 PROFILE_HOTKEY_ID_BASE + 索引
	std::vector<_ProfileShortcutInfo> _profileShortcutInfos;
	static constexpr int PROFILE_HOTKEY_ID_BASE = 0x100;
	wil::unique_hwnd _hwndHotkey;
	wil::unique_hhook _keyboardHook;

	Event<winrt::Magpie::ShortcutAction>::EventRevoker _shortcutChangedRevoker;
	Event<>::EventRevoker _profileShortcutChangedRevoker;
	Event<Profile&>::EventRevoker _profileAddedRevoker;
	Event<uint32_t>::EventRevoker _profileRemovedRevoker;
	Event<uint32_t, bool>::EventRevoker _profileMovedRevoker;

	bool _isKeyboardHookActive = true;
	// 用于防止长按时重复触发热键
	bool _keyboardHookShortcutActivated = false;
};

}
