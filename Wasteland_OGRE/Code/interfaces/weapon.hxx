#include "StdAfx.h"
// Copyright (C) 2005-2010 Code Synthesis Tools CC
//
// This program was generated by CodeSynthesis XSD, an XML Schema to
// C++ data binding compiler.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
// In addition, as a special exception, Code Synthesis Tools CC gives
// permission to link this program with the Xerces-C++ library (or with
// modified versions of Xerces-C++ that use the same license as Xerces-C++),
// and distribute linked combinations including the two. You must obey
// the GNU General Public License version 2 in all respects for all of
// the code used other than Xerces-C++. If you modify this copy of the
// program, you may extend this exception to your version of the program,
// but you are not obligated to do so. If you do not wish to do so, delete
// this exception statement from your version.
//
// Furthermore, Code Synthesis Tools CC makes a special exception for
// the Free/Libre and Open Source Software (FLOSS) which is described
// in the accompanying FLOSSE file.
//

#ifndef E__OGRE_BUILD_BIN_DEBUG_RESOURCE_XML_SCHEMAS_WEAPON_HXX
#define E__OGRE_BUILD_BIN_DEBUG_RESOURCE_XML_SCHEMAS_WEAPON_HXX

// Begin prologue.
//
//
// End prologue.

#include <xsd/cxx/config.hxx>

#if (XSD_INT_VERSION != 3030000L)
#error XSD runtime version mismatch
#endif

#include <xsd/cxx/pre.hxx>

#ifndef XSD_USE_CHAR
#define XSD_USE_CHAR
#endif

#ifndef XSD_CXX_TREE_USE_CHAR
#define XSD_CXX_TREE_USE_CHAR
#endif

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/types.hxx>

#include <xsd/cxx/xml/error-handler.hxx>

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

#include <xsd/cxx/tree/parsing.hxx>
#include <xsd/cxx/tree/parsing/byte.hxx>
#include <xsd/cxx/tree/parsing/unsigned-byte.hxx>
#include <xsd/cxx/tree/parsing/short.hxx>
#include <xsd/cxx/tree/parsing/unsigned-short.hxx>
#include <xsd/cxx/tree/parsing/int.hxx>
#include <xsd/cxx/tree/parsing/unsigned-int.hxx>
#include <xsd/cxx/tree/parsing/long.hxx>
#include <xsd/cxx/tree/parsing/unsigned-long.hxx>
#include <xsd/cxx/tree/parsing/boolean.hxx>
#include <xsd/cxx/tree/parsing/float.hxx>
#include <xsd/cxx/tree/parsing/double.hxx>
#include <xsd/cxx/tree/parsing/decimal.hxx>

namespace xml_schema
{
  // anyType and anySimpleType.
  //
  typedef ::xsd::cxx::tree::type type;
  typedef ::xsd::cxx::tree::simple_type< type > simple_type;
  typedef ::xsd::cxx::tree::type container;

  // 8-bit
  //
  typedef signed char byte;
  typedef unsigned char unsigned_byte;

  // 16-bit
  //
  typedef short short_;
  typedef unsigned short unsigned_short;

  // 32-bit
  //
  typedef int int_;
  typedef unsigned int unsigned_int;

  // 64-bit
  //
  typedef long long long_;
  typedef unsigned long long unsigned_long;

  // Supposed to be arbitrary-length integral types.
  //
  typedef long long integer;
  typedef long long non_positive_integer;
  typedef unsigned long long non_negative_integer;
  typedef unsigned long long positive_integer;
  typedef long long negative_integer;

  // Boolean.
  //
  typedef bool boolean;

  // Floating-point types.
  //
  typedef float float_;
  typedef double double_;
  typedef double decimal;

  // String types.
  //
  typedef ::xsd::cxx::tree::string< char, simple_type > string;
  typedef ::xsd::cxx::tree::normalized_string< char, string > normalized_string;
  typedef ::xsd::cxx::tree::token< char, normalized_string > token;
  typedef ::xsd::cxx::tree::name< char, token > name;
  typedef ::xsd::cxx::tree::nmtoken< char, token > nmtoken;
  typedef ::xsd::cxx::tree::nmtokens< char, simple_type, nmtoken > nmtokens;
  typedef ::xsd::cxx::tree::ncname< char, name > ncname;
  typedef ::xsd::cxx::tree::language< char, token > language;

  // ID/IDREF.
  //
  typedef ::xsd::cxx::tree::id< char, ncname > id;
  typedef ::xsd::cxx::tree::idref< char, ncname, type > idref;
  typedef ::xsd::cxx::tree::idrefs< char, simple_type, idref > idrefs;

  // URI.
  //
  typedef ::xsd::cxx::tree::uri< char, simple_type > uri;

  // Qualified name.
  //
  typedef ::xsd::cxx::tree::qname< char, simple_type, uri, ncname > qname;

  // Binary.
  //
  typedef ::xsd::cxx::tree::buffer< char > buffer;
  typedef ::xsd::cxx::tree::base64_binary< char, simple_type > base64_binary;
  typedef ::xsd::cxx::tree::hex_binary< char, simple_type > hex_binary;

  // Date/time.
  //
  typedef ::xsd::cxx::tree::time_zone time_zone;
  typedef ::xsd::cxx::tree::date< char, simple_type > date;
  typedef ::xsd::cxx::tree::date_time< char, simple_type > date_time;
  typedef ::xsd::cxx::tree::duration< char, simple_type > duration;
  typedef ::xsd::cxx::tree::gday< char, simple_type > gday;
  typedef ::xsd::cxx::tree::gmonth< char, simple_type > gmonth;
  typedef ::xsd::cxx::tree::gmonth_day< char, simple_type > gmonth_day;
  typedef ::xsd::cxx::tree::gyear< char, simple_type > gyear;
  typedef ::xsd::cxx::tree::gyear_month< char, simple_type > gyear_month;
  typedef ::xsd::cxx::tree::time< char, simple_type > time;

  // Entity.
  //
  typedef ::xsd::cxx::tree::entity< char, ncname > entity;
  typedef ::xsd::cxx::tree::entities< char, simple_type, entity > entities;

  // Flags and properties.
  //
  typedef ::xsd::cxx::tree::flags flags;
  typedef ::xsd::cxx::tree::properties< char > properties;

  // Parsing/serialization diagnostics.
  //
  typedef ::xsd::cxx::tree::severity severity;
  typedef ::xsd::cxx::tree::error< char > error;
  typedef ::xsd::cxx::tree::diagnostics< char > diagnostics;

  // Exceptions.
  //
  typedef ::xsd::cxx::tree::exception< char > exception;
  typedef ::xsd::cxx::tree::bounds< char > bounds;
  typedef ::xsd::cxx::tree::duplicate_id< char > duplicate_id;
  typedef ::xsd::cxx::tree::parsing< char > parsing;
  typedef ::xsd::cxx::tree::expected_element< char > expected_element;
  typedef ::xsd::cxx::tree::unexpected_element< char > unexpected_element;
  typedef ::xsd::cxx::tree::expected_attribute< char > expected_attribute;
  typedef ::xsd::cxx::tree::unexpected_enumerator< char > unexpected_enumerator;
  typedef ::xsd::cxx::tree::expected_text_content< char > expected_text_content;
  typedef ::xsd::cxx::tree::no_prefix_mapping< char > no_prefix_mapping;

  // Error handler callback interface.
  //
  typedef ::xsd::cxx::xml::error_handler< char > error_handler;

  // DOM interaction.
  //
  namespace dom
  {
    // Automatic pointer for DOMDocument.
    //
    using ::xsd::cxx::xml::dom::auto_ptr;

#ifndef XSD_CXX_TREE_TREE_NODE_KEY__XML_SCHEMA
#define XSD_CXX_TREE_TREE_NODE_KEY__XML_SCHEMA
    // DOM user data key for back pointers to tree nodes.
    //
    const XMLCh* const tree_node_key = ::xsd::cxx::tree::user_data_keys::node;
#endif
  }
}

// Forward declarations.
//
class sounds_t;
class soundFrame_t;
class soundFrames_t;
class animationFrame_t;
class animationFrames_t;
class gameplay_t;
class weapon_t;

#include <memory>    // std::auto_ptr
#include <limits>    // std::numeric_limits
#include <algorithm> // std::binary_search

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/containers.hxx>
#include <xsd/cxx/tree/list.hxx>

#include <xsd/cxx/xml/dom/parsing-header.hxx>

class sounds_t: public ::xml_schema::type
{
  public:
  // fire
  // 
  typedef ::xml_schema::string fire_type;
  typedef ::xsd::cxx::tree::traits< fire_type, char > fire_traits;

  const fire_type&
  fire () const;

  fire_type&
  fire ();

  void
  fire (const fire_type& x);

  void
  fire (::std::auto_ptr< fire_type > p);

  // reload
  // 
  typedef ::xml_schema::string reload_type;
  typedef ::xsd::cxx::tree::traits< reload_type, char > reload_traits;

  const reload_type&
  reload () const;

  reload_type&
  reload ();

  void
  reload (const reload_type& x);

  void
  reload (::std::auto_ptr< reload_type > p);

  // putaway
  // 
  typedef ::xml_schema::string putaway_type;
  typedef ::xsd::cxx::tree::traits< putaway_type, char > putaway_traits;

  const putaway_type&
  putaway () const;

  putaway_type&
  putaway ();

  void
  putaway (const putaway_type& x);

  void
  putaway (::std::auto_ptr< putaway_type > p);

  // dryfire
  // 
  typedef ::xml_schema::string dryfire_type;
  typedef ::xsd::cxx::tree::traits< dryfire_type, char > dryfire_traits;

  const dryfire_type&
  dryfire () const;

  dryfire_type&
  dryfire ();

  void
  dryfire (const dryfire_type& x);

  void
  dryfire (::std::auto_ptr< dryfire_type > p);

  // altfire
  // 
  typedef ::xml_schema::string altfire_type;
  typedef ::xsd::cxx::tree::traits< altfire_type, char > altfire_traits;

  const altfire_type&
  altfire () const;

  altfire_type&
  altfire ();

  void
  altfire (const altfire_type& x);

  void
  altfire (::std::auto_ptr< altfire_type > p);

  // altreload
  // 
  typedef ::xml_schema::string altreload_type;
  typedef ::xsd::cxx::tree::traits< altreload_type, char > altreload_traits;

  const altreload_type&
  altreload () const;

  altreload_type&
  altreload ();

  void
  altreload (const altreload_type& x);

  void
  altreload (::std::auto_ptr< altreload_type > p);

  // Constructors.
  //
  sounds_t (const fire_type&,
            const reload_type&,
            const putaway_type&,
            const dryfire_type&,
            const altfire_type&,
            const altreload_type&);

  sounds_t (const ::xercesc::DOMElement& e,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  sounds_t (const sounds_t& x,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  virtual sounds_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  virtual 
  ~sounds_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< fire_type > fire_;
  ::xsd::cxx::tree::one< reload_type > reload_;
  ::xsd::cxx::tree::one< putaway_type > putaway_;
  ::xsd::cxx::tree::one< dryfire_type > dryfire_;
  ::xsd::cxx::tree::one< altfire_type > altfire_;
  ::xsd::cxx::tree::one< altreload_type > altreload_;
};

class soundFrame_t: public ::xml_schema::type
{
  public:
  // frame
  // 
  typedef ::xml_schema::integer frame_type;
  typedef ::xsd::cxx::tree::traits< frame_type, char > frame_traits;

  const frame_type&
  frame () const;

  frame_type&
  frame ();

  void
  frame (const frame_type& x);

  // sound
  // 
  typedef ::xml_schema::string sound_type;
  typedef ::xsd::cxx::tree::traits< sound_type, char > sound_traits;

  const sound_type&
  sound () const;

  sound_type&
  sound ();

  void
  sound (const sound_type& x);

  void
  sound (::std::auto_ptr< sound_type > p);

  // Constructors.
  //
  soundFrame_t (const frame_type&,
                const sound_type&);

  soundFrame_t (const ::xercesc::DOMElement& e,
                ::xml_schema::flags f = 0,
                ::xml_schema::container* c = 0);

  soundFrame_t (const soundFrame_t& x,
                ::xml_schema::flags f = 0,
                ::xml_schema::container* c = 0);

  virtual soundFrame_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  virtual 
  ~soundFrame_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< frame_type > frame_;
  ::xsd::cxx::tree::one< sound_type > sound_;
};

class soundFrames_t: public ::xml_schema::type
{
  public:
  // soundframe
  // 
  typedef ::soundFrame_t soundframe_type;
  typedef ::xsd::cxx::tree::sequence< soundframe_type > soundframe_sequence;
  typedef soundframe_sequence::iterator soundframe_iterator;
  typedef soundframe_sequence::const_iterator soundframe_const_iterator;
  typedef ::xsd::cxx::tree::traits< soundframe_type, char > soundframe_traits;

  const soundframe_sequence&
  soundframe () const;

  soundframe_sequence&
  soundframe ();

  void
  soundframe (const soundframe_sequence& s);

  // Constructors.
  //
  soundFrames_t ();

  soundFrames_t (const ::xercesc::DOMElement& e,
                 ::xml_schema::flags f = 0,
                 ::xml_schema::container* c = 0);

  soundFrames_t (const soundFrames_t& x,
                 ::xml_schema::flags f = 0,
                 ::xml_schema::container* c = 0);

  virtual soundFrames_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  virtual 
  ~soundFrames_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  soundframe_sequence soundframe_;
};

class animationFrame_t: public ::xml_schema::type
{
  public:
  // animationName
  // 
  typedef ::xml_schema::string animationName_type;
  typedef ::xsd::cxx::tree::traits< animationName_type, char > animationName_traits;

  const animationName_type&
  animationName () const;

  animationName_type&
  animationName ();

  void
  animationName (const animationName_type& x);

  void
  animationName (::std::auto_ptr< animationName_type > p);

  // begin
  // 
  typedef ::xml_schema::integer begin_type;
  typedef ::xsd::cxx::tree::traits< begin_type, char > begin_traits;

  const begin_type&
  begin () const;

  begin_type&
  begin ();

  void
  begin (const begin_type& x);

  // end
  // 
  typedef ::xml_schema::integer end_type;
  typedef ::xsd::cxx::tree::traits< end_type, char > end_traits;

  const end_type&
  end () const;

  end_type&
  end ();

  void
  end (const end_type& x);

  // Constructors.
  //
  animationFrame_t (const animationName_type&,
                    const begin_type&,
                    const end_type&);

  animationFrame_t (const ::xercesc::DOMElement& e,
                    ::xml_schema::flags f = 0,
                    ::xml_schema::container* c = 0);

  animationFrame_t (const animationFrame_t& x,
                    ::xml_schema::flags f = 0,
                    ::xml_schema::container* c = 0);

  virtual animationFrame_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  virtual 
  ~animationFrame_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< animationName_type > animationName_;
  ::xsd::cxx::tree::one< begin_type > begin_;
  ::xsd::cxx::tree::one< end_type > end_;
};

class animationFrames_t: public ::xml_schema::type
{
  public:
  // animationFrame
  // 
  typedef ::animationFrame_t animationFrame_type;
  typedef ::xsd::cxx::tree::sequence< animationFrame_type > animationFrame_sequence;
  typedef animationFrame_sequence::iterator animationFrame_iterator;
  typedef animationFrame_sequence::const_iterator animationFrame_const_iterator;
  typedef ::xsd::cxx::tree::traits< animationFrame_type, char > animationFrame_traits;

  const animationFrame_sequence&
  animationFrame () const;

  animationFrame_sequence&
  animationFrame ();

  void
  animationFrame (const animationFrame_sequence& s);

  // Constructors.
  //
  animationFrames_t ();

  animationFrames_t (const ::xercesc::DOMElement& e,
                     ::xml_schema::flags f = 0,
                     ::xml_schema::container* c = 0);

  animationFrames_t (const animationFrames_t& x,
                     ::xml_schema::flags f = 0,
                     ::xml_schema::container* c = 0);

  virtual animationFrames_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  virtual 
  ~animationFrames_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  animationFrame_sequence animationFrame_;
};

class gameplay_t: public ::xml_schema::type
{
  public:
  // accuracy
  // 
  typedef ::xml_schema::integer accuracy_type;
  typedef ::xsd::cxx::tree::traits< accuracy_type, char > accuracy_traits;

  const accuracy_type&
  accuracy () const;

  accuracy_type&
  accuracy ();

  void
  accuracy (const accuracy_type& x);

  // firerate
  // 
  typedef ::xml_schema::integer firerate_type;
  typedef ::xsd::cxx::tree::traits< firerate_type, char > firerate_traits;

  const firerate_type&
  firerate () const;

  firerate_type&
  firerate ();

  void
  firerate (const firerate_type& x);

  // range
  // 
  typedef ::xml_schema::integer range_type;
  typedef ::xsd::cxx::tree::traits< range_type, char > range_traits;

  const range_type&
  range () const;

  range_type&
  range ();

  void
  range (const range_type& x);

  // reloadQty
  // 
  typedef ::xml_schema::integer reloadQty_type;
  typedef ::xsd::cxx::tree::traits< reloadQty_type, char > reloadQty_traits;

  const reloadQty_type&
  reloadQty () const;

  reloadQty_type&
  reloadQty ();

  void
  reloadQty (const reloadQty_type& x);

  // damage
  // 
  typedef ::xml_schema::integer damage_type;
  typedef ::xsd::cxx::tree::traits< damage_type, char > damage_traits;

  const damage_type&
  damage () const;

  damage_type&
  damage ();

  void
  damage (const damage_type& x);

  // Constructors.
  //
  gameplay_t (const accuracy_type&,
              const firerate_type&,
              const range_type&,
              const reloadQty_type&,
              const damage_type&);

  gameplay_t (const ::xercesc::DOMElement& e,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  gameplay_t (const gameplay_t& x,
              ::xml_schema::flags f = 0,
              ::xml_schema::container* c = 0);

  virtual gameplay_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  virtual 
  ~gameplay_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< accuracy_type > accuracy_;
  ::xsd::cxx::tree::one< firerate_type > firerate_;
  ::xsd::cxx::tree::one< range_type > range_;
  ::xsd::cxx::tree::one< reloadQty_type > reloadQty_;
  ::xsd::cxx::tree::one< damage_type > damage_;
};

class weapon_t: public ::xml_schema::type
{
  public:
  // name
  // 
  typedef ::xml_schema::string name_type;
  typedef ::xsd::cxx::tree::traits< name_type, char > name_traits;

  const name_type&
  name () const;

  name_type&
  name ();

  void
  name (const name_type& x);

  void
  name (::std::auto_ptr< name_type > p);

  // type
  // 
  typedef ::xml_schema::string type_type;
  typedef ::xsd::cxx::tree::traits< type_type, char > type_traits;

  const type_type&
  type () const;

  type_type&
  type ();

  void
  type (const type_type& x);

  void
  type (::std::auto_ptr< type_type > p);

  // model
  // 
  typedef ::xml_schema::string model_type;
  typedef ::xsd::cxx::tree::traits< model_type, char > model_traits;

  const model_type&
  model () const;

  model_type&
  model ();

  void
  model (const model_type& x);

  void
  model (::std::auto_ptr< model_type > p);

  // materialFile
  // 
  typedef ::xml_schema::string materialFile_type;
  typedef ::xsd::cxx::tree::traits< materialFile_type, char > materialFile_traits;

  const materialFile_type&
  materialFile () const;

  materialFile_type&
  materialFile ();

  void
  materialFile (const materialFile_type& x);

  void
  materialFile (::std::auto_ptr< materialFile_type > p);

  // sounds
  // 
  typedef ::sounds_t sounds_type;
  typedef ::xsd::cxx::tree::traits< sounds_type, char > sounds_traits;

  const sounds_type&
  sounds () const;

  sounds_type&
  sounds ();

  void
  sounds (const sounds_type& x);

  void
  sounds (::std::auto_ptr< sounds_type > p);

  // soundFrames
  // 
  typedef ::soundFrames_t soundFrames_type;
  typedef ::xsd::cxx::tree::traits< soundFrames_type, char > soundFrames_traits;

  const soundFrames_type&
  soundFrames () const;

  soundFrames_type&
  soundFrames ();

  void
  soundFrames (const soundFrames_type& x);

  void
  soundFrames (::std::auto_ptr< soundFrames_type > p);

  // animationFrames
  // 
  typedef ::animationFrames_t animationFrames_type;
  typedef ::xsd::cxx::tree::traits< animationFrames_type, char > animationFrames_traits;

  const animationFrames_type&
  animationFrames () const;

  animationFrames_type&
  animationFrames ();

  void
  animationFrames (const animationFrames_type& x);

  void
  animationFrames (::std::auto_ptr< animationFrames_type > p);

  // gameplay
  // 
  typedef ::gameplay_t gameplay_type;
  typedef ::xsd::cxx::tree::traits< gameplay_type, char > gameplay_traits;

  const gameplay_type&
  gameplay () const;

  gameplay_type&
  gameplay ();

  void
  gameplay (const gameplay_type& x);

  void
  gameplay (::std::auto_ptr< gameplay_type > p);

  // Constructors.
  //
  weapon_t (const name_type&,
            const type_type&,
            const model_type&,
            const materialFile_type&,
            const sounds_type&,
            const soundFrames_type&,
            const animationFrames_type&,
            const gameplay_type&);

  weapon_t (const name_type&,
            const type_type&,
            const model_type&,
            const materialFile_type&,
            ::std::auto_ptr< sounds_type >&,
            ::std::auto_ptr< soundFrames_type >&,
            ::std::auto_ptr< animationFrames_type >&,
            ::std::auto_ptr< gameplay_type >&);

  weapon_t (const ::xercesc::DOMElement& e,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  weapon_t (const weapon_t& x,
            ::xml_schema::flags f = 0,
            ::xml_schema::container* c = 0);

  virtual weapon_t*
  _clone (::xml_schema::flags f = 0,
          ::xml_schema::container* c = 0) const;

  virtual 
  ~weapon_t ();

  // Implementation.
  //
  protected:
  void
  parse (::xsd::cxx::xml::dom::parser< char >&,
         ::xml_schema::flags);

  protected:
  ::xsd::cxx::tree::one< name_type > name_;
  ::xsd::cxx::tree::one< type_type > type_;
  ::xsd::cxx::tree::one< model_type > model_;
  ::xsd::cxx::tree::one< materialFile_type > materialFile_;
  ::xsd::cxx::tree::one< sounds_type > sounds_;
  ::xsd::cxx::tree::one< soundFrames_type > soundFrames_;
  ::xsd::cxx::tree::one< animationFrames_type > animationFrames_;
  ::xsd::cxx::tree::one< gameplay_type > gameplay_;
};

#include <iosfwd>

#include <xercesc/sax/InputSource.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>

// Parse a URI or a local file.
//

::std::auto_ptr< ::weapon_t >
weapon (const ::std::string& uri,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::weapon_t >
weapon (const ::std::string& uri,
        ::xml_schema::error_handler& eh,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::weapon_t >
weapon (const ::std::string& uri,
        ::xercesc::DOMErrorHandler& eh,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse std::istream.
//

::std::auto_ptr< ::weapon_t >
weapon (::std::istream& is,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::weapon_t >
weapon (::std::istream& is,
        ::xml_schema::error_handler& eh,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::weapon_t >
weapon (::std::istream& is,
        ::xercesc::DOMErrorHandler& eh,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::weapon_t >
weapon (::std::istream& is,
        const ::std::string& id,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::weapon_t >
weapon (::std::istream& is,
        const ::std::string& id,
        ::xml_schema::error_handler& eh,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::weapon_t >
weapon (::std::istream& is,
        const ::std::string& id,
        ::xercesc::DOMErrorHandler& eh,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse xercesc::InputSource.
//

::std::auto_ptr< ::weapon_t >
weapon (::xercesc::InputSource& is,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::weapon_t >
weapon (::xercesc::InputSource& is,
        ::xml_schema::error_handler& eh,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::weapon_t >
weapon (::xercesc::InputSource& is,
        ::xercesc::DOMErrorHandler& eh,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

// Parse xercesc::DOMDocument.
//

::std::auto_ptr< ::weapon_t >
weapon (const ::xercesc::DOMDocument& d,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

::std::auto_ptr< ::weapon_t >
weapon (::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument >& d,
        ::xml_schema::flags f = 0,
        const ::xml_schema::properties& p = ::xml_schema::properties ());

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

#endif // E__OGRE_BUILD_BIN_DEBUG_RESOURCE_XML_SCHEMAS_WEAPON_HXX
