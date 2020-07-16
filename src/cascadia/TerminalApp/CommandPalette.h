// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include "CommandPalette.g.h"
#include "../../cascadia/inc/cppwinrt_utils.h"

namespace winrt::TerminalApp::implementation
{
    enum class CommandPaletteMode
    {
        ActionMode = 0,
        TabSwitcherMode
    };

    struct CommandPalette : CommandPaletteT<CommandPalette>
    {
        CommandPalette();

        Windows::Foundation::Collections::IObservableVector<TerminalApp::Command> FilteredActions();

        void SetCommands(Windows::Foundation::Collections::IVector<TerminalApp::Command> const& actions);
        void EnableCommandPaletteMode();

        void SetDispatch(const winrt::TerminalApp::ShortcutActionDispatch& dispatch);

        // Tab Switcher 
        void EnableTabSwitcherMode(const Windows::System::VirtualKey& anchorKey, const uint32_t startIdx);
        void OnTabsChanged(const Windows::Foundation::IInspectable& s, const Windows::Foundation::Collections::IVectorChangedEventArgs& e);

        WINRT_CALLBACK(PropertyChanged, Windows::UI::Xaml::Data::PropertyChangedEventHandler);
        OBSERVABLE_GETSET_PROPERTY(winrt::hstring, NoMatchesText, _PropertyChangedHandlers);
        OBSERVABLE_GETSET_PROPERTY(winrt::hstring, SearchBoxText, _PropertyChangedHandlers);

    private:
        friend struct CommandPaletteT<CommandPalette>; // for Xaml to bind events

        Windows::Foundation::Collections::IObservableVector<TerminalApp::Command> _filteredActions{ nullptr };

        Windows::Foundation::Collections::IVector<TerminalApp::Command> _allCommands{ nullptr };
        winrt::TerminalApp::ShortcutActionDispatch _dispatch;

        Windows::Foundation::Collections::IVector<TerminalApp::Command> _commandsToFilter();

        void _filterTextChanged(Windows::Foundation::IInspectable const& sender,
                                Windows::UI::Xaml::RoutedEventArgs const& args);
        void _previewKeyDownHandler(Windows::Foundation::IInspectable const& sender,
                                    Windows::UI::Xaml::Input::KeyRoutedEventArgs const& e);
        void _keyDownHandler(Windows::Foundation::IInspectable const& sender,
                             Windows::UI::Xaml::Input::KeyRoutedEventArgs const& e);
        void _keyUpHandler(Windows::Foundation::IInspectable const& sender,
                           Windows::UI::Xaml::Input::KeyRoutedEventArgs const& e);

        void _rootPointerPressed(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e);
        void _backdropPointerPressed(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e);

        void _listItemClicked(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::Controls::ItemClickEventArgs const& e);

        void _selectNextItem(const bool moveDown);

        void _updateFilteredActions();
        static int _getWeight(const winrt::hstring& searchText, const winrt::hstring& name);
        void _close();

        CommandPaletteMode _currentMode;
        void _switchToMode(CommandPaletteMode mode);

        // Tab Switcher
        std::optional<winrt::Windows::System::VirtualKey> _anchorKey;
        void GenerateCommandForTab(const uint32_t idx, bool inserted, winrt::TerminalApp::Tab& tab);
        void UpdateTabIndices(const uint32_t startIdx);
        Windows::Foundation::Collections::IVector<TerminalApp::Command> _allTabActions{ nullptr };
        uint32_t _switcherStartIdx;

        void _dispatchCommand(const TerminalApp::Command& command);

        void _dismissPalette();
    };
}

namespace winrt::TerminalApp::factory_implementation
{
    BASIC_FACTORY(CommandPalette);
}
