<?php

# read a network short i bytes into amf
function get_short($amf, $i) {
	return (ord($amf[$i]) << 8) | ord($amf[$i + 1]);
}

# read a network long i bytes into amf
function get_long($amf, $i) {
	return (ord($amf[$i]) << 24) | (ord($amf[$i + 1]) << 16) | (ord($amf[$i + 2]) << 8) | ord($amf[$i + 3]);
}

# encode an amf counted string WITHOUT type byte
function make_string($str) {
	$len = strlen($str);
	return chr(($len >> 8) & 0xff) . chr($len & 0xff) . $str;
}

# encode an int as 32-bit big-endian
function make_long($i) {
	return chr(($i >> 24) & 0xff) . chr(($i >> 16) & 0xff) . chr(($i >> 8) & 0xff) . chr($i & 0xff);
}

# pass: binary data
# returns: hex encoded like this: de:ad:be:ef
function hexify($bin) {
	$len = strlen($bin);
	$ret = '';
	for($i = 0; $i < $len; ++$i) {
		$ret .= sprintf('%02x:', ord($bin[$i]));
	}
	return substr($ret, 0, -1);
}


# pass: name of remote procedure, AMF encoded args
# returns: string
function amf_1st_arg_type($message_name, $amf) {
	if(ord($amf[0]) != 0x0a) {
		return "couldn't find args";
	} else {
		$i = 1; # done with first byte
		$num_args = get_long($amf, $i); $i += 4;
		if($num_args == 0) {
			return "got 0 args";
		}

		switch(ord($amf[$i])) {
			case 0x00: return "DOUBLE";
			case 0x01: return "BOOLEAN";
			case 0x02: return "STRING";
			case 0x03: return "OBJECT";
			case 0x04: return "MOVIE_CLIP";
			case 0x05: return "NUL";
			case 0x06: return "UNDEFINED";
			case 0x07: return "REFERENCE";
			case 0x08: return "ECMA_ARRAY";
			case 0x09: return "OBJECT_END";
			case 0x0a: return "STRICT_ARRAY";
			case 0x0b: return "DATE";
			case 0x0c: return "LONG_STRING";
			case 0x0d: return "UNSUPPORTED";
			case 0x0e: return "RECORD_SET";
			case 0x0f: return "XML_OBJECT";
			case 0x10: return "TYPED_OBJECT";
			case 0x11: return "AMF3_DATA";

			default: return "unknown type";
		}
	}
}

function raw_rpc_to_array($request_id, $message_name, $amf) {
	$ret = array();
	$ret['type'] = amf_1st_arg_type($message_name, $amf);
	$ret['hex'] = hexify($amf);
	$ret['message'] = $message_name;
	$ret['request_id'] = $request_id;
	return $ret;
}


function make_ecma($o) {
	$ret = make_long(count($o)); # FIME what's this supposed to be? the highest integer key?
	foreach($o as $key => $val) {
		$ret .= make_string('' . $key);
		$ret .= make_val($val);
	}
	$ret .= "\000\000\011"; #object end
	return $ret;
}

# return $val encoded as AMF with type byte
function make_val($val) {
	if(is_array($val)) {
		return chr(0x08) . make_ecma($val);
	} else {
		return chr(0x02) . make_string($val);
	}
}

# pass: name of remote procedure, AMF encoded args
# returns: amf reply
function raw_rpc($request_id, $message_name, $amf) {
	$ret = '';
	$ary = raw_rpc_to_array($request_id, $message_name, $amf);
	$ret .= make_val($ary);
	return $ret;
}

function echo_main() {
	$amf = $GLOBALS['HTTP_RAW_POST_DATA'];
	$reply = '';
	$num_replies = 0;
	$i = 2; # skip version bytes
	$headers = get_short($amf, $i); $i += 2;
	while($headers) {
		# FIXME
		--$headers;
	}

	$bodies = get_short($amf, $i); $i += 2;
	while($bodies && $i < strlen($amf)) {
		$len = get_short($amf, $i);  $i += 2;
		$message_name = substr($amf, $i, $len); $i += $len; # this is the name of the remote procedure requested
		$len = get_short($amf, $i);  $i += 2;
		$request_id = substr($amf, $i, $len); $i += $len; # eg /1
		$len = get_long($amf, $i); $i += 4; # total size in bytes
		$answer = raw_rpc($request_id, $message_name, substr($amf, $i, $len)); $i += $len;

		if($request_id != '/') {
			$num_replies += 1;
			$reply .= make_string($request_id . '/onResult');
			$reply .= make_string('null');
			$reply .= "\377\377\377\377"; # should be size of reply in bytes, but this works
			$reply .= $answer;
		}
		
		--$bodies;
	}

	$reply = "\000\000\000\000" # version number, client ID, number of headers
	       . chr(($num_replies >> 8) & 0xff) . chr($num_replies & 0xff) # rumber of replies
	       . $reply;

	print($reply);
	exit();
}

echo_main();

?>