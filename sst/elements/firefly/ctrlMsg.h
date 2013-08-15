
// Copyright 2009-2013 Sandia Corporation. Under the terms
// of Contract DE-AC04-94AL85000 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
// 
// Copyright (c) 2009-2013, Sandia Corporation
// All rights reserved.
// 
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

#ifndef COMPONENTS_FIREFLY_CTRLMSG_H
#define COMPONENTS_FIREFLY_CTRLMSG_H

#include "sst/core/output.h"
#include "protocolAPI.h"

namespace SST {
namespace Firefly {

class Info;

class CtrlMsg : public ProtocolAPI {
  public: 
    struct IoVec {
        void*  ptr;
        size_t len;
    };

  private:

    struct Hdr {
        size_t  len;
        int     tag;
        int     src;
        int     group;
    };

    struct BaseInfo {
        BaseInfo( std::vector<IoVec> _ioVec, int _tag, int _group) : 
            ioVec( _ioVec ),
            tag( _tag ),
            group( _group ),
            len( 0 )
        {
            for ( unsigned int i; i < ioVec.size(); i++ ) {
                len += ioVec[i].len;
            }
        }

        virtual ~BaseInfo() {} 
        std::vector<IoVec> ioVec;
        int     tag;
        int     group;
        size_t  len;
    };

    struct RecvInfo : public BaseInfo {
        RecvInfo( std::vector<IoVec> _ioVec, int _src, int _tag, int _group) : 
            BaseInfo( _ioVec, _tag, _group ),
            src( _src ) 
        {}
        int     src;
    };

    struct SendInfo : public BaseInfo {
        SendInfo( std::vector<IoVec> ioVec, int _dest, int _tag, int _group) : 
            BaseInfo( ioVec, _tag, _group ),
            dest( _dest ) 
        {}
        int     dest;
    };

  public:


    struct CommReq {
        CommReq() : info( NULL ) {}
        BaseInfo*    info;
        bool done;
    };

  private:

    class SendReq: public Request {
      public:
        Hdr         hdr;
        CommReq*    commReq;
    };

    class RecvReq: public Request {
      public:
        enum { RecvHdr, RecvBody } state;
        Hdr         hdr;
        CommReq*    commReq;
        std::vector<unsigned char> buf;
    };

  public:

    CtrlMsg( int verboseLevel, Output::output_location_t loc, Info* );

    virtual SendReq* getSendReq( );
    virtual RecvReq* getRecvReq( IO::NodeId src );
    virtual SendReq* sendIODone( Request* );
    virtual RecvReq* recvIODone( Request* );
    virtual Request* delayDone( Request* );

    void recv( void* buf, size_t len, int src,  int tag, int group, CommReq* );
    void recvv( std::vector<IoVec>&, int src,  int tag, int group, CommReq* );
    void send( void* buf, size_t len, int dest, int tag, int group, CommReq* );
    void sendv( std::vector<IoVec>&, int dest, int tag, int group, CommReq* );
    bool test( CommReq*  );
    void setup();


  private:

    CommReq* findMatch( Hdr& );
    RecvReq* searchUnexpected( RecvInfo& info );

    Info* m_info;
    std::deque< CommReq* > m_sendQ;
    std::deque< CommReq* > m_postedQ;
    std::deque< RecvReq* > m_unexpectedQ;
};

}
}

#endif
