#pragma once
#include "NewProfileViewModel.g.h"

namespace winrt::Magpie::implementation {

struct NewProfileViewModel : NewProfileViewModelT<NewProfileViewModel>,
                             wil::notify_property_changed_base<NewProfileViewModel> {
	NewProfileViewModel() = default;

	void PrepareForOpen();

	IVector<IInspectable> CandidateWindows() const noexcept {
		return _candidateWindows;
	}

	int CandidateWindowIndex() const noexcept {
		return _candidateWindowIndex;
	}

	void CandidateWindowIndex(int value);

	hstring Name() const noexcept {
		return _name;
	}

	void Name(const hstring& value) noexcept;

	IVector<IInspectable> Profiles() const noexcept {
		return _profiles;
	}

	int ProfileIndex() const noexcept {
		return _profileIndex;
	}

	void ProfileIndex(int value) {
		_profileIndex = value;
		RaisePropertyChanged(L"ProfileIndex");
	}

	bool IsConfirmButtonEnabled() const noexcept {
		return _isConfirmButtonEnabled;
	}

	// 用于显示“没有可选窗口”的提示，通用配置文件不需要窗口
	bool IsNoCandidateWindow() const noexcept {
		return !_isGeneralProfile && (!_candidateWindows || _candidateWindows.Size() == 0);
	}

	// 用于显示表单
	bool IsAnyCandidateWindow() const noexcept {
		return !IsNoCandidateWindow();
	}

	bool IsGeneralProfile() const noexcept {
		return _isGeneralProfile;
	}

	void IsGeneralProfile(bool value);

	bool IsNotGeneralProfile() const noexcept {
		return !_isGeneralProfile;
	}

	void Confirm() const noexcept;

private:
	void _UpdateIsConfirmButtonEnabled() noexcept {
		_IsConfirmButtonEnabled(!_name.empty() && (_isGeneralProfile || _candidateWindowIndex >= 0));
	}

	void _IsConfirmButtonEnabled(bool value) noexcept {
		if (_isConfirmButtonEnabled == value) {
			return;
		}

		_isConfirmButtonEnabled = value;
		RaisePropertyChanged(L"IsConfirmButtonEnabled");
	}

	IVector<IInspectable> _candidateWindows{ nullptr };
	int _candidateWindowIndex = -1;
	hstring _name;
	IVector<IInspectable> _profiles{ nullptr };
	int _profileIndex = 0;
	bool _isConfirmButtonEnabled = false;
	bool _isGeneralProfile = false;
};

}
