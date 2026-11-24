#pragma once
#include "Event.h"
#include "Singleton.h"

namespace Magpie {

struct Profile;
struct Shortcut;

class ProfileService : public Singleton<ProfileService> {
	friend Singleton<ProfileService>;

public:
	bool TestNewProfile(bool isPackaged, std::wstring_view pathOrAumid, std::wstring_view className) noexcept;

	// copyFrom < 0 表示复制默认配置
	bool AddProfile(bool isPackaged, std::wstring_view pathOrAumid, std::wstring_view className, std::wstring_view name, int copyFrom);

	// 添加不关联任何窗口的通用配置文件，copyFrom < 0 表示复制默认配置
	void AddGeneralProfile(std::wstring_view name, int copyFrom);

	void RenameProfile(uint32_t profileIdx, std::wstring_view newName);

	void RemoveProfile(uint32_t profileIdx);

	bool MoveProfile(uint32_t profileIdx, bool isMoveUp);

	const Profile* GetProfileForWindow(HWND hWnd, bool forAutoScale) noexcept;

	Profile& DefaultProfile() noexcept;

	Profile& GetProfile(uint32_t idx) noexcept;

	uint32_t GetProfileCount() noexcept;

	// profileIdx < 0 表示默认配置
	Profile& GetProfileOrDefault(int profileIdx) noexcept;

	const Shortcut& GetProfileShortcut(int profileIdx, bool windowedMode) noexcept;

	void SetProfileShortcut(int profileIdx, bool windowedMode, const Shortcut& value);

	Event<Profile&> ProfileAdded;
	Event<uint32_t> ProfileRenamed;
	Event<uint32_t> ProfileRemoved;
	Event<uint32_t, bool> ProfileMoved;
	// 任一配置文件的热键改变
	Event<> ProfileShortcutChanged;

private:
	ProfileService() = default;
};

}
