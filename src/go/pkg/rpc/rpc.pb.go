// Code generated by protoc-gen-go from "rpc.proto"
// DO NOT EDIT!

package org_esb_rpc

import proto "goprotobuf.googlecode.com/hg/proto"
import "math"
import "os"

// Reference proto, math & os imports to suppress error if they are not otherwise used.
var _ = proto.GetString
var _ = math.Inf
var _ os.Error

type ErrorReason int32
const (
	ErrorReason_NO_ERROR_OCCURRED = 0
	ErrorReason_BAD_REQUEST_DATA = 1
	ErrorReason_BAD_REQUEST_PROTO = 2
	ErrorReason_SERVICE_NOT_FOUND = 3
	ErrorReason_METHOD_NOT_FOUND = 4
	ErrorReason_RPC_ERROR = 5
	ErrorReason_RPC_FAILED = 6
	ErrorReason_INVALID_REQUEST_PROTO = 7
	ErrorReason_BAD_RESPONSE_PROTO = 8
	ErrorReason_UNKNOWN_HOST = 9
	ErrorReason_IO_ERROR = 10
)
var ErrorReason_name = map[int32] string {
	0: "NO_ERROR_OCCURRED",
	1: "BAD_REQUEST_DATA",
	2: "BAD_REQUEST_PROTO",
	3: "SERVICE_NOT_FOUND",
	4: "METHOD_NOT_FOUND",
	5: "RPC_ERROR",
	6: "RPC_FAILED",
	7: "INVALID_REQUEST_PROTO",
	8: "BAD_RESPONSE_PROTO",
	9: "UNKNOWN_HOST",
	10: "IO_ERROR",
}
var ErrorReason_value = map[string] int32 {
	"NO_ERROR_OCCURRED": 0,
	"BAD_REQUEST_DATA": 1,
	"BAD_REQUEST_PROTO": 2,
	"SERVICE_NOT_FOUND": 3,
	"METHOD_NOT_FOUND": 4,
	"RPC_ERROR": 5,
	"RPC_FAILED": 6,
	"INVALID_REQUEST_PROTO": 7,
	"BAD_RESPONSE_PROTO": 8,
	"UNKNOWN_HOST": 9,
	"IO_ERROR": 10,
}
func NewErrorReason(x int32) *ErrorReason {
	e := ErrorReason(x)
	return &e
}

type Request struct {
	ServiceName	*string	"PB(bytes,1,req,name=service_name)"
	MethodName	*string	"PB(bytes,2,req,name=method_name)"
	RequestProto	[]byte	"PB(bytes,3,req,name=request_proto)"
	XXX_unrecognized	[]byte
}
func (this *Request) Reset() {
	*this = Request{}
}

type Response struct {
	ResponseProto	[]byte	"PB(bytes,1,opt,name=response_proto)"
	Error	*string	"PB(bytes,2,opt,name=error)"
	Callback	*bool	"PB(varint,3,opt,name=callback,def=0)"
	ErrorReason	*ErrorReason	"PB(varint,4,opt,name=error_reason,enum=org_esb_rpc.ErrorReason)"
	XXX_unrecognized	[]byte
}
func (this *Response) Reset() {
	*this = Response{}
}
const Default_Response_Callback bool = false

type QueryForServiceRequest struct {
	ServiceName	*string	"PB(bytes,1,req,name=service_name)"
	XXX_unrecognized	[]byte
}
func (this *QueryForServiceRequest) Reset() {
	*this = QueryForServiceRequest{}
}

type QueryForServiceResponse struct {
	ServiceName	*string	"PB(bytes,1,opt,name=service_name)"
	Registered	*bool	"PB(varint,2,req,name=registered,def=0)"
	XXX_unrecognized	[]byte
}
func (this *QueryForServiceResponse) Reset() {
	*this = QueryForServiceResponse{}
}
const Default_QueryForServiceResponse_Registered bool = false

type Node struct {
	Address	*string	"PB(bytes,1,req,name=address)"
	Name	*string	"PB(bytes,2,req,name=name)"
	XXX_unrecognized	[]byte
}
func (this *Node) Reset() {
	*this = Node{}
}

type NodeList struct {
	Nodes	[]*Node	"PB(bytes,1,rep,name=nodes)"
	XXX_unrecognized	[]byte
}
func (this *NodeList) Reset() {
	*this = NodeList{}
}

type ProcessUnit struct {
	SerializedData	[]byte	"PB(bytes,1,req,name=serialized_data)"
	XXX_unrecognized	[]byte
}
func (this *ProcessUnit) Reset() {
	*this = ProcessUnit{}
}

type ProcessUnitRequest struct {
	Node	*Node	"PB(bytes,1,req,name=node)"
	UnitData	*ProcessUnit	"PB(bytes,2,opt,name=unit_data)"
	XXX_unrecognized	[]byte
}
func (this *ProcessUnitRequest) Reset() {
	*this = ProcessUnitRequest{}
}

type ProcessUnitResponse struct {
	UnitData	*ProcessUnit	"PB(bytes,2,opt,name=unit_data)"
	Status	*string	"PB(bytes,3,req,name=status)"
	XXX_unrecognized	[]byte
}
func (this *ProcessUnitResponse) Reset() {
	*this = ProcessUnitResponse{}
}

type Process struct {
	ProcessName	*string	"PB(bytes,1,req,name=process_name)"
	XXX_unrecognized	[]byte
}
func (this *Process) Reset() {
	*this = Process{}
}

type ProcessRequest struct {
	Node	*Node	"PB(bytes,1,req,name=node)"
	ProcessData	*Process	"PB(bytes,2,opt,name=process_data)"
	XXX_unrecognized	[]byte
}
func (this *ProcessRequest) Reset() {
	*this = ProcessRequest{}
}

type ProcessResponse struct {
	ProcessData	*Process	"PB(bytes,2,opt,name=process_data)"
	StatusMessage	*string	"PB(bytes,3,req,name=status_message)"
	StatusCode	*int32	"PB(varint,4,req,name=status_code)"
	XXX_unrecognized	[]byte
}
func (this *ProcessResponse) Reset() {
	*this = ProcessResponse{}
}

func init() {
	proto.RegisterEnum("org_esb_rpc.ErrorReason", ErrorReason_name, ErrorReason_value)
}
