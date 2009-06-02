// TextFormatAlign_as.hx:  ActionScript 3 "TextFormatAlign" class, for Gnash.
//
// Generated by gen-as3.sh on: 20090515 by "rob". Remove this
// after any hand editing loosing changes.
//
//   Copyright (C) 2009 Free Software Foundation, Inc.
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
//

// This test case must be processed by CPP before compiling to include the
//  DejaGnu.hx header file for the testing framework support.

#if flash9
import flash.text.TextFormatAlign;
import flash.display.MovieClip;
#else
//import flash.TextFormatAlign;
//import flash.MovieClip;
#end
import flash.Lib;
import Type;

// import our testing API
import DejaGnu;

// Class must be named with the _as suffix, as that's the same name as the file.
class TextFormatAlign_as {
    static function main() {

#if flash9
//        var x1:TextFormatAlign = new TextFormatAlign();

        // Make sure we actually get a valid class        
        if (Type.typeof(TextFormatAlign)==TObject) {
            DejaGnu.pass("TextFormatAlign class exists");
        } else {
            DejaGnu.fail("TextFormatAlign class doesn't exist");
        }
	if (Std.is(TextFormatAlign.CENTER,String) ) {
            DejaGnu.pass("TextFormatAlign CENTER exists");
        } else {
            DejaGnu.fail("TextFormatAlign CENTER doesn't exist");
        }
	if (Std.is(TextFormatAlign.JUSTIFY,String) ){
            DejaGnu.pass("TextFormatAlign JUSTIFY exists");
        } else {
            DejaGnu.fail("TextFormatAlign JUSTIFY doesn't exist");
        }
	if (Std.is(TextFormatAlign.LEFT,String) ){
            DejaGnu.pass("TextFormatAlign LEFT exists");
        } else {
            DejaGnu.fail("TextFormatAlign LEFT doesn't exist");
        }
	if (Std.is(TextFormatAlign.RIGHT,String) ){
            DejaGnu.pass("TextFormatAlign RIGHT exists");
        } else {
            DejaGnu.fail("TextFormatAlign RIGHT doesn't exist");
        }

// Tests to see if all the methods exist. All these do is test for
// existance of a method, and don't test the functionality at all. This
// is primarily useful only to test completeness of the API implementation.
#end

        // Call this after finishing all tests. It prints out the totals.
        DejaGnu.done();
    }
}

// local Variables:
// mode: C++
// indent-tabs-mode: t
// End:

