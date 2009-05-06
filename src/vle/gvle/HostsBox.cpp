/**
 * @file vle/gvle/HostsBox.cpp
 * @author The VLE Development Team
 */

/*
 * VLE Environment - the multimodeling and simulation environment
 * This file is a part of the VLE environment (http://vle.univ-littoral.fr)
 * Copyright (C) 2003 - 2008 The VLE Development Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <vle/gvle/HostsBox.hpp>
#include <vle/gvle/Message.hpp>
#include <vle/utils/XML.hpp>
#include <vle/utils/Tools.hpp>
#include <vle/utils/Socket.hpp>
#include <libxml++/libxml++.h>
#include <glibmm/thread.h>
#include <gtkmm/stock.h>

using namespace vle;

namespace vle
{
namespace gvle {

HostInformation::HostInformation(Glib::RefPtr < Gnome::Glade::Xml > ref):
        mThread(0),
        mThreadRun(false),
        mPing(false),
        mDaemon(false),
        mRefXML(ref)
{
    mRefXML->get_widget("DialogHostInformation", mDialog);
    mRefXML->get_widget("entryHostname", mEntry);
    mRefXML->get_widget("spinbuttonPort", mSpinButtonPort);
    mRefXML->get_widget("spinbuttonProcessNumber", mSpinButtonProcess);
    mRefXML->get_widget("labelConnection", mLabelConnection);
    mRefXML->get_widget("labelDaemon", mLabelDaemon);
    mRefXML->get_widget("imageConnection", mImageConnection);
    mRefXML->get_widget("imageDaemon", mImageDaemon);
}

HostInformation::~HostInformation()
{
}

bool HostInformation::run(utils::Host& host)
{
    bool r = false;

    mDialog->show_all();

    try {
        mThread = Glib::Thread::create(
                      sigc::mem_fun(*this, &HostInformation::thread), true);

        mThreadRun = true;

        sigc::slot<bool> myslot = sigc::bind(
                                      sigc::mem_fun(*this, &HostInformation::timer), 0);
        mTimer = Glib::signal_timeout().connect(myslot, 2000);

    } catch (const Glib::ThreadError& e) {
        gvle::Error((boost::format(
                         "Thread building error: %1%") % e.what()).str());
    }

    if (mDialog->run() == Gtk::RESPONSE_OK) {
        host.hostname(mEntry->get_text());
        host.port((int)mSpinButtonPort->get_value());
        host.process((int)mSpinButtonProcess->get_value());
        r = true;
    }

    mDialog->hide_all();
    close();

    return r;
}

void HostInformation::clear()
{
    mEntry->set_text("");
    mSpinButtonPort->set_value(8000);
    mSpinButtonProcess->set_value(1);
    mLabelConnection->set_text("Connection... waiting");
    mLabelDaemon->set_text("Daemon... waiting");
    mImageConnection->set(Gtk::Stock::CLOSE, Gtk::ICON_SIZE_BUTTON);
    mImageDaemon->set(Gtk::Stock::CLOSE, Gtk::ICON_SIZE_BUTTON);
}

void HostInformation::init(const utils::Host& host)
{
    mEntry->set_text(host.hostname());
    mSpinButtonPort->set_value(host.port());
    mSpinButtonProcess->set_value(host.process());
    mLabelConnection->set_text("Connection... waiting");
    mLabelDaemon->set_text("Daemon... waiting");
    mImageConnection->set(Gtk::Stock::CLOSE, Gtk::ICON_SIZE_BUTTON);
    mImageDaemon->set(Gtk::Stock::CLOSE, Gtk::ICON_SIZE_BUTTON);
}

void HostInformation::thread()
{
    while (mThreadRun) {
        mWaiting = false;
        utils::net::Client* client = 0;

        try {
            client = new utils::net::Client(mIP, mPort);
            client->send_buffer("exit");
            client->close();
            delete client;
            mPing = true;
            mDaemon = true;
        } catch (std::exception& e) {
            if (client)
                delete client;
            mPing = false;
            mDaemon = false;
        }
        mWaiting = true;
        Glib::usleep(500000);
    }
}

bool HostInformation::timer(int /* t */)
{
    mIP = mEntry->get_text();
    mPort = (int)mSpinButtonPort->get_value();

    if (not mPing)
        mImageConnection->set(Gtk::Stock::CLOSE, Gtk::ICON_SIZE_BUTTON);
    else
        mImageConnection->set(Gtk::Stock::APPLY, Gtk::ICON_SIZE_BUTTON);

    if (not mDaemon)
        mImageDaemon->set(Gtk::Stock::CLOSE, Gtk::ICON_SIZE_BUTTON);
    else
        mImageDaemon->set(Gtk::Stock::APPLY, Gtk::ICON_SIZE_BUTTON);

    return true;
}

void HostInformation::close()
{
    if (mThreadRun) {
        mThreadRun = false;
        mThread->join();
        mThread = 0;
    }

    if (mTimer.connected()) {
        mTimer.disconnect();
    }
}

HostsBox::HostsBox(Glib::RefPtr < Gnome::Glade::Xml > ref) :
        mHostInformation(ref),
        mRefXML(ref)
{
    mRefXML = Gnome::Glade::Xml::create(
                  utils::Path::path().getGladeFile("gvle.glade"));

    mRefXML->get_widget("DialogHosts", mDialog);
    mRefXML->get_widget("addButtonHost", mAdd);
    mRefXML->get_widget("delButtonHost", mDel);
    mRefXML->get_widget("okbuttonHosts", mOk);
    mRefXML->get_widget("cancelbuttonHosts", mCancel);
    mRefXML->get_widget("treeviewHosts", mTreeView);
    mRefXML->get_widget("informationsLabelFrameHost", mLabel);

    mListStore = Gtk::ListStore::create(mColumn);
    mTreeView->set_model(mListStore);
    mTreeView->append_column("Host", mColumn.mHostname);
    mTreeView->append_column("Port", mColumn.mPort);
    mTreeView->append_column("Process", mColumn.mProcess);

    init_hosts();

    mAdd->signal_clicked().connect(sigc::mem_fun(
                                       *this, &HostsBox::addHost));
    mDel->signal_clicked().connect(sigc::mem_fun(
                                       *this, &HostsBox::delHost));
    mTreeView->signal_row_activated().connect(sigc::mem_fun(
                *this, &HostsBox::modifyHost));

    mDialog->show_all();
}

HostsBox::~HostsBox()
{
    delete mDialog;
}

void HostsBox::run()
{
    if (mDialog->run() == Gtk::RESPONSE_OK) {
        mHosts.write_file();
    }
}

int HostsBox::test_vle_existence(const Glib::ustring&, int)
{
    // FIXME
    return -1;
}

bool HostsBox::build_only_localhost_user_hosts_file()
{
    Glib::ustring filename(get_hosts_filename());
    if (utils::exist_file(filename) == false) {
        xmlpp::Document doc;
        xmlpp::Element* root = doc.create_root_node("HOSTS");
        xmlpp::Element* host = root->add_child("HOST");
        host->set_attribute("HOSTNAME", "localhost");
        host->set_attribute("PORT", "8000");
        host->set_attribute("PROCESS", "1");
        doc.write_to_file_formatted(filename);
        return true;
    }
    return false;
}

Glib::ustring HostsBox::get_hosts_filename()
{
    return utils::Path::path().getHomeFile("hosts.xml");
}

void HostsBox::init_hosts()
{
    try {
        mHosts.read_file();
    } catch (const utils::InternalError& e) {
        gvle::Error((boost::format(
                         "Error parsing file %1%\n%2%.") %
                     utils::Hosts::get_hosts_filename() %
                     e.what()).str());
    } catch (const xmlpp::exception& e) {
        gvle::Error((boost::format(
                         "Error reading file %1%\n%2%.") %
                     utils::Hosts::get_hosts_filename() %
                     e.what()).str());
    }
    redraw();
}

void HostsBox::addHost()
{
    utils::Host newhost;
    mHostInformation.clear();
    if (mHostInformation.run(newhost) == true) {
        mHosts.push_host(newhost);
        redraw();
    }
}

void HostsBox::modifyHost(const Gtk::TreeModel::Path& /*path*/,
                          Gtk::TreeViewColumn* /*column*/)
{
    Glib::RefPtr < Gtk::TreeSelection > selection = mTreeView->get_selection();
    Gtk::TreeModel::iterator it = selection->get_selected();
    if (it) {
        Gtk::TreeModel::Row row = *it;
        utils::Host newhost(mHosts.get_host(row[mColumn.mHostname]));
        mHosts.remove_host(row[mColumn.mHostname]);
        mHostInformation.init(newhost);
        mHostInformation.run(newhost);
        mHosts.push_host(newhost);
        redraw();
    }
}

void HostsBox::delHost()
{
    Glib::RefPtr < Gtk::TreeSelection > selection = mTreeView->get_selection();
    Gtk::TreeModel::iterator it = selection->get_selected();
    if (it) {
        Gtk::TreeModel::Row row = *it;
        mHosts.remove_host(row[mColumn.mHostname]);
        mListStore->erase(it);
    }
    redraw();
}

void HostsBox::redraw()
{
    mListStore->clear();
    utils::Hosts::SetHosts::const_iterator it = mHosts.hosts().begin();
    while (it != mHosts.hosts().end()) {
        Gtk::TreeModel::Row row = *(mListStore->append());
        if (row) {
            row[mColumn.mHostname] = (*it).hostname();
            row[mColumn.mPort] = (*it).port();
            row[mColumn.mProcess] = (*it).process();
        }
        ++it;
    }
}

}
} // namespace vle gvle
