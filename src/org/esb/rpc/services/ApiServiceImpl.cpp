/*
 * File:   ApiService.cpp
 * Author: HoelscJ
 *
 * Created on 4. April 2011, 15:05
 */

#include "ApiServiceImpl.h"
namespace org {
  namespace esb {
    namespace rpc {

      ApiServiceImpl::ApiServiceImpl(Server * s) : _db(org::esb::hive::DatabaseService::getDatabase()) {

      }

      void ApiServiceImpl::authenticate(::google::protobuf::RpcController* controller,
      const ::org::esb::rpc::User* request,
      ::org::esb::rpc::AuthToken* response,
      ::google::protobuf::Closure* done){
        _authtoken="bla";
        response->set_token(_authtoken);
      }

      void ApiServiceImpl::getMediafile(::google::protobuf::RpcController* controller,
      const ::org::esb::rpc::MediafileRequest* request,
      ::org::esb::rpc::MediafileResponse* response,
      ::google::protobuf::Closure* done) {

      }

      void ApiServiceImpl::addMediafile(::google::protobuf::RpcController* controller,
      const ::org::esb::rpc::MediafileRequest* request,
      ::org::esb::rpc::MediafileResponse* response,
      ::google::protobuf::Closure* done) {

      }

      void ApiServiceImpl::getMediafileList(::google::protobuf::RpcController* controller,
      const ::org::esb::rpc::MediafileRequest* request,
      ::org::esb::rpc::MediafileResponse* response,
      ::google::protobuf::Closure* done) {
        if(_authtoken.length()==0){
          controller->SetFailed("not authenticated");
          return;
        }else{
          LOGDEBUG("Token="<<_authtoken);
        }

        LOGDEBUG(boost::this_thread::get_id());
        vector<db::MediaFile>files = litesql::select<db::MediaFile > (_db).all();
        vector<db::MediaFile>::iterator it = files.begin();
        for (; it != files.end(); it++) {
          Mediafile * file = response->mutable_mediafiles()->Add();
          file->set_id((*it).id);
          file->set_filename((*it).filename);
          file->set_size((*it).filesize);
          file->set_duration((*it).duration);
          file->set_format((*it).containertype);
          file->set_stream_count((*it).streamcount);
        }
        response->set_status(0);
      }

      ApiServiceImpl::~ApiServiceImpl() {
      }

    }
  }
}
