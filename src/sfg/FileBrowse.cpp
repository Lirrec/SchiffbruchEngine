#include "sbe/sfg/FileBrowse.hpp"

#include "sbe/Engine.hpp"
#include "sbe/Config.hpp"
#include "sbe/sfg/List.hpp"

#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Label.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <boost/filesystem.hpp>

namespace sbe {

    FileBrowse::FileBrowse ( const glm::ivec2 Size, const select_type t, const std::string& title,
                             const std::string& okEvent, const std::string& cancelEvent )
        : okEvt ( okEvent ), cancelEvt ( cancelEvent ), okAction(  [](const std::string&){} ), cancelAction( []{} ), selection_type(t)
    {
        RegisterForEvent ( "FB_SEL_CHANGED" );
        RegisterForEvent ( "KEY_PRESSED_ENTER" );
        //RegisterForEvent();

        CreateWin ( Size, title );
        Win->Show ( false );
        Module::Get()->QueueEvent ( Event ( "SCREEN_ADD_WINDOW", Win ) );
    }

    void FileBrowse::CreateWin ( const glm::ivec2 Size, const std::string& title )
    {
        Win = sfg::Window::Create ( sfg::Window::Style::BACKGROUND | sfg::Window::Style::TITLEBAR | sfg::Window::Style::SHADOW );

        fldrLst = std::make_shared<sbe::sfgList> ( "FB_SEL_CHANGED" );
        sfg::Box::Ptr    box ( sfg::Box::Create ( sfg::Box::Orientation::VERTICAL, 3.0f ) );
        sfg::Box::Ptr    btnBox ( sfg::Box::Create ( sfg::Box::Orientation::HORIZONTAL, 3.0f ) );
        errorLabel = sfg::Label::Create( " " );
        sfg::Button::Ptr btnOK ( sfg::Button::Create ( "OK" ) );
        sfg::Button::Ptr btnCancel ( sfg::Button::Create ( "Cancel" ) );
        etyLoc = sfg::Entry::Create();
        etyLoc->SetText ( boost::filesystem::current_path().string() );

        btnOK->GetSignal ( sfg::Button::OnLeftClick ).Connect ( std::bind( &FileBrowse::onOkClicked, this) );
        btnCancel->GetSignal ( sfg::Button::OnLeftClick ).Connect ( std::bind( &FileBrowse::onCancelClicked, this) );

        Win->SetRequisition ( sf::Vector2f ( Size.x,Size.y ) );
        fldrLst->getList()->SetRequisition ( { 20, 300} );
        updatePosition();

        btnBox->Pack ( errorLabel,  true, true );
        btnBox->Pack ( btnCancel, false, false );
        btnBox->Pack ( btnOK,     false, false );
        box->Pack ( etyLoc, false, false );
        box->Pack ( fldrLst->getList(),   false, true );
        box->Pack ( btnBox, false, false );

        Win->SetTitle ( title );
        Win->Add ( box );

        updateFolderList ( etyLoc->GetText() );
    }


    void FileBrowse::HandleEvent ( Event& e )
    {
        if ( e.Is ( "FB_SEL_CHANGED", typeid ( std::string ) ) )
            updateFolderList ( boost::any_cast<std::string> ( e.Data() ) );
        else if ( e.Is ( "KEY_PRESSED_ENTER" ) && Win->IsGloballyVisible() )
            updateFolderList ( std::string ( etyLoc->GetText() ) );
    }


    void FileBrowse::setTitle ( const std::string& t )
    {
        Win->SetTitle ( t );
    }

    void FileBrowse::setOkEvt ( const std::string& e )
    {
        okEvt = e;
    }

    void FileBrowse::setCancelEvt ( const std::string& e )
    {
        cancelEvt = e;
    }

    void FileBrowse::setOkAction(const std::function<void(const std::string&)> &f)
    {
        okAction = f;
    }

    void FileBrowse::setCancelAction(const std::function<void()> &f)
    {
        cancelAction = f;
    }

    void FileBrowse::show()
    {
        updatePosition();
        Win->Show ( true );
        Win->GrabFocus();
        //Engine::out() << "[FileBrowser] show" << std::endl;
    }

    void FileBrowse::hide()
    {
    	errorLabel->SetText(" ");
        Win->Show ( false );
        //Engine::out() << "[FileBrowser] hide" << std::endl;
    }


    void FileBrowse::updatePosition()
    {
        sf::FloatRect Allocation = Win->GetAllocation();
        sf::Vector2u winSize =  Engine::GetApp().getSize();
        Win->SetPosition ( sf::Vector2f ( ( winSize.x - Allocation.width ) /2 , ( winSize.y - Allocation.height ) /2   ) );
    }

    void FileBrowse::onOkClicked()
    {
        using boost::filesystem::path;
    	path file = std::string(etyLoc->GetText());
    	if ( selection_type == select_type::File && !is_regular_file(file) )
		{
			Engine::out() << "[FileBrowse] Selected element is no file!" << std::endl;
			errorLabel->SetText("Selected element is no file!");
			return;
		}
		if ( selection_type == select_type::Directory && !is_directory(file) )
		{
			Engine::out() << "[FileBrowse] Selected element is no directory!" << std::endl;
			errorLabel->SetText("Selected element is no directory!");
			return;
		}

        okAction(etyLoc->GetText());
        Module::Get()->QueueEvent ( Event ( okEvt, std::string ( etyLoc->GetText() ) ), true );
        hide();
    }

    void FileBrowse::onCancelClicked()
    {
        cancelAction();
        Module::Get()->QueueEvent ( Event ( cancelEvt ) );
        hide();
    }


    void FileBrowse::updateFolderList ( const std::string& p )
    {
        using boost::filesystem::path;
        using boost::filesystem::directory_iterator;
        // make list empty
        fldrLst->clear();

        path bp;

        if ( p == ".." )
        {
            // path = parent of lastpath
            bp = lastPath.parent_path();
        }
        else
        {
            bp = path ( p );
            if ( !bp.is_absolute() )
            {
                bp = lastPath / bp;
            }
        }
        //Engine::out() << "[FileBrowser] new path is »" << bp.string() << "«"<< std::endl;

        if ( exists ( bp ) && is_directory ( bp ) )
        {
            lastPath = bp;
            fldrLst->addItem ( ".." ); // always provide a way back up
            etyLoc->SetText ( bp.string() );

            // read folder content
            for ( auto dit=directory_iterator ( bp ); dit != directory_iterator(); ++dit )
            {
                // if isDir or files should be selected, addItem
                if ( selection_type == select_type::File || is_directory ( dit->path() ) )
                    fldrLst->addItem ( dit->path().filename().string() );
            }

        }
        else
        {
        	if ( selection_type == select_type::File)
			{
				lastPath = bp;
				fldrLst->addItem ( ".." ); // always provide a way back up
				etyLoc->SetText ( bp.string() );
			}
			//else
			//	Engine::out() << "[FileBrowser] " << bp.string() << " exists: " << exists ( bp ) << ", is_dir: "<< is_directory ( bp ) << std::endl;
            //fldrLst->addItem ( "This folder doesn't exist. Click to create... (nyi!!)" );
        }
    }

} // namespace sbe
