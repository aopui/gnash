// 
//   Copyright (C) 2005, 2006, 2007, 2008, 2009 Free Software Foundation, Inc.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef _RTMP_SERVER_H_
#define _RTMP_SERVER_H_

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <map>

#include "rtmp.h"
#include "amf.h"
#include "handler.h"
#include "network.h"
#include "buffer.h"
#include "diskstream.h"
#include "rtmp_msg.h"
#include "dsodefs.h"

namespace cygnal
{

class RTMPServer : public gnash::RTMP
{
public:
    RTMPServer();
    ~RTMPServer();

    /// \method 
    ///     This method is called after the initial network connection
    ///     is established. It reads in the handshake from the client,
    ///     responds appropriately, and then extracts the initial AMF
    ///     object, which is always of type NetConnection, doing an
    ///     INVOKE operation of ::connect(). serverFinish() is
    ///     actually used to extract the AMF data from the packet, and
    ///     handShakeResponse() is used to construct the response packet.
    boost::shared_ptr<amf::Element> processClientHandShake(int fd);

    bool packetSend(amf::Buffer &buf);
    bool packetRead(amf::Buffer &buf);
    
    // These are handlers for the various types
    boost::shared_ptr<amf::Buffer> encodeResult(gnash::RTMPMsg::rtmp_status_e status);
    boost::shared_ptr<amf::Buffer> encodeResult(gnash::RTMPMsg::rtmp_status_e status, const std::string &filename);
    boost::shared_ptr<amf::Buffer> encodeResult(gnash::RTMPMsg::rtmp_status_e status, const std::string &filename, double &streamid);
    boost::shared_ptr<amf::Buffer> encodeResult(gnash::RTMPMsg::rtmp_status_e status, double &streamid);
    boost::shared_ptr<amf::Buffer> encodeResult(gnash::RTMPMsg::rtmp_status_e status, const std::string &filename, double &streamid, double &clientid);
    boost::shared_ptr<amf::Buffer> encodePing(rtmp_ping_e type, boost::uint32_t milliseconds);
    boost::shared_ptr<amf::Buffer> encodePing(rtmp_ping_e type);

    // Parse an Echo Request message coming from the Red5 echo_test.
    std::vector<boost::shared_ptr<amf::Element > > parseEchoRequest(amf::Buffer &buf) { return parseEchoRequest(buf.reference(), buf.size()); };
    std::vector<boost::shared_ptr<amf::Element > > parseEchoRequest(boost::uint8_t *buf, size_t size);
    // format a response to the 'echo' test used for testing Gnash.
    boost::shared_ptr<amf::Buffer> formatEchoResponse(double num, amf::Element &el);
    boost::shared_ptr<amf::Buffer> formatEchoResponse(double num, amf::Buffer &data);
    boost::shared_ptr<amf::Buffer> formatEchoResponse(double num, boost::uint8_t *data, size_t size);    
    void addReference(boost::uint16_t index, amf::Element &el) { _references[index] = el; };
    amf::Element &getReference(boost::uint16_t index) { return _references[index]; };

    bool sendFile(int fd, const std::string &filespec);

    double createClientID();
    double createStreamID();

    void setStreamID(double id) { _streamid = id; };
    double getStreamID() { return _streamid; };

    size_t sendToClient(std::vector<int> &fds, boost::uint8_t *data,
			size_t size);
    size_t sendToClient(std::vector<int> &fds,amf::Buffer &data);

    void dump();

private:
    /// \method serverFinish
    ///     This is only called by processClientHandshake() to compare
    ///     the handshakes to make sure they match, and to extract the
    ///     initial AMF data from packet.
    boost::shared_ptr<amf::Buffer> serverFinish(int fd,
			amf::Buffer &handshake1, amf::Buffer &handshake2);
    /// \method handShakeResponse
    ///     This is only called by processClientHandshake() to
    ///     construct the handshake response to the client.
    bool handShakeResponse(int fd, amf::Buffer &buf);
    
    /// This is used by the boost tokenizer functions, and is defined
    /// here purely for convienience.
    typedef boost::char_separator<char> Sep;
    typedef boost::tokenizer<Sep> Tok;


    gnash::DiskStream::filetype_e  _filetype;
    std::string		_docroot;
    std::string		_filespec;
    boost::uint32_t     _filesize;
    std::map<boost::uint16_t, amf::Element> _references;
    std::vector<double>	_clientids;
    double		_streamid;
};

// This is the thread for all incoming RTMP connections
bool DSOEXPORT rtmp_handler(gnash::Network::thread_params_t *args);

} // end of gnash namespace
// end of _RTMP_SERVER_H_
#endif

// local Variables:
// mode: C++
// indent-tabs-mode: t
// End:

