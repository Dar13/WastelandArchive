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

// Begin prologue.
//
//
// End prologue.

#include <xsd/cxx/pre.hxx>

#include "configuration.hxx"

// action_t
// 

const action_t::reload_type& action_t::
reload () const
{
  return this->reload_.get ();
}

action_t::reload_type& action_t::
reload ()
{
  return this->reload_.get ();
}

void action_t::
reload (const reload_type& x)
{
  this->reload_.set (x);
}

void action_t::
reload (::std::auto_ptr< reload_type > x)
{
  this->reload_.set (x);
}

const action_t::envwarnsys_type& action_t::
envwarnsys () const
{
  return this->envwarnsys_.get ();
}

action_t::envwarnsys_type& action_t::
envwarnsys ()
{
  return this->envwarnsys_.get ();
}

void action_t::
envwarnsys (const envwarnsys_type& x)
{
  this->envwarnsys_.set (x);
}

void action_t::
envwarnsys (::std::auto_ptr< envwarnsys_type > x)
{
  this->envwarnsys_.set (x);
}

const action_t::use_type& action_t::
use () const
{
  return this->use_.get ();
}

action_t::use_type& action_t::
use ()
{
  return this->use_.get ();
}

void action_t::
use (const use_type& x)
{
  this->use_.set (x);
}

void action_t::
use (::std::auto_ptr< use_type > x)
{
  this->use_.set (x);
}


// movement_t
// 

const movement_t::forward_type& movement_t::
forward () const
{
  return this->forward_.get ();
}

movement_t::forward_type& movement_t::
forward ()
{
  return this->forward_.get ();
}

void movement_t::
forward (const forward_type& x)
{
  this->forward_.set (x);
}

void movement_t::
forward (::std::auto_ptr< forward_type > x)
{
  this->forward_.set (x);
}

const movement_t::backward_type& movement_t::
backward () const
{
  return this->backward_.get ();
}

movement_t::backward_type& movement_t::
backward ()
{
  return this->backward_.get ();
}

void movement_t::
backward (const backward_type& x)
{
  this->backward_.set (x);
}

void movement_t::
backward (::std::auto_ptr< backward_type > x)
{
  this->backward_.set (x);
}

const movement_t::right_type& movement_t::
right () const
{
  return this->right_.get ();
}

movement_t::right_type& movement_t::
right ()
{
  return this->right_.get ();
}

void movement_t::
right (const right_type& x)
{
  this->right_.set (x);
}

void movement_t::
right (::std::auto_ptr< right_type > x)
{
  this->right_.set (x);
}

const movement_t::left_type& movement_t::
left () const
{
  return this->left_.get ();
}

movement_t::left_type& movement_t::
left ()
{
  return this->left_.get ();
}

void movement_t::
left (const left_type& x)
{
  this->left_.set (x);
}

void movement_t::
left (::std::auto_ptr< left_type > x)
{
  this->left_.set (x);
}

const movement_t::sprint_type& movement_t::
sprint () const
{
  return this->sprint_.get ();
}

movement_t::sprint_type& movement_t::
sprint ()
{
  return this->sprint_.get ();
}

void movement_t::
sprint (const sprint_type& x)
{
  this->sprint_.set (x);
}

void movement_t::
sprint (::std::auto_ptr< sprint_type > x)
{
  this->sprint_.set (x);
}

const movement_t::jump_type& movement_t::
jump () const
{
  return this->jump_.get ();
}

movement_t::jump_type& movement_t::
jump ()
{
  return this->jump_.get ();
}

void movement_t::
jump (const jump_type& x)
{
  this->jump_.set (x);
}

void movement_t::
jump (::std::auto_ptr< jump_type > x)
{
  this->jump_.set (x);
}


// volume_t
// 

const volume_t::character_type& volume_t::
character () const
{
  return this->character_.get ();
}

volume_t::character_type& volume_t::
character ()
{
  return this->character_.get ();
}

void volume_t::
character (const character_type& x)
{
  this->character_.set (x);
}

const volume_t::music_type& volume_t::
music () const
{
  return this->music_.get ();
}

volume_t::music_type& volume_t::
music ()
{
  return this->music_.get ();
}

void volume_t::
music (const music_type& x)
{
  this->music_.set (x);
}

const volume_t::soundfx_type& volume_t::
soundfx () const
{
  return this->soundfx_.get ();
}

volume_t::soundfx_type& volume_t::
soundfx ()
{
  return this->soundfx_.get ();
}

void volume_t::
soundfx (const soundfx_type& x)
{
  this->soundfx_.set (x);
}


// configuration_t
// 

const configuration_t::action_type& configuration_t::
action () const
{
  return this->action_.get ();
}

configuration_t::action_type& configuration_t::
action ()
{
  return this->action_.get ();
}

void configuration_t::
action (const action_type& x)
{
  this->action_.set (x);
}

void configuration_t::
action (::std::auto_ptr< action_type > x)
{
  this->action_.set (x);
}

const configuration_t::movement_type& configuration_t::
movement () const
{
  return this->movement_.get ();
}

configuration_t::movement_type& configuration_t::
movement ()
{
  return this->movement_.get ();
}

void configuration_t::
movement (const movement_type& x)
{
  this->movement_.set (x);
}

void configuration_t::
movement (::std::auto_ptr< movement_type > x)
{
  this->movement_.set (x);
}

const configuration_t::volume_type& configuration_t::
volume () const
{
  return this->volume_.get ();
}

configuration_t::volume_type& configuration_t::
volume ()
{
  return this->volume_.get ();
}

void configuration_t::
volume (const volume_type& x)
{
  this->volume_.set (x);
}

void configuration_t::
volume (::std::auto_ptr< volume_type > x)
{
  this->volume_.set (x);
}


#include <xsd/cxx/xml/dom/parsing-source.hxx>

// action_t
//

action_t::
action_t (const reload_type& reload,
          const envwarnsys_type& envwarnsys,
          const use_type& use)
: ::xml_schema::type (),
  reload_ (reload, ::xml_schema::flags (), this),
  envwarnsys_ (envwarnsys, ::xml_schema::flags (), this),
  use_ (use, ::xml_schema::flags (), this)
{
}

action_t::
action_t (const action_t& x,
          ::xml_schema::flags f,
          ::xml_schema::container* c)
: ::xml_schema::type (x, f, c),
  reload_ (x.reload_, f, this),
  envwarnsys_ (x.envwarnsys_, f, this),
  use_ (x.use_, f, this)
{
}

action_t::
action_t (const ::xercesc::DOMElement& e,
          ::xml_schema::flags f,
          ::xml_schema::container* c)
: ::xml_schema::type (e, f | ::xml_schema::flags::base, c),
  reload_ (f, this),
  envwarnsys_ (f, this),
  use_ (f, this)
{
  if ((f & ::xml_schema::flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false);
    this->parse (p, f);
  }
}

void action_t::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::flags f)
{
  for (; p.more_elements (); p.next_element ())
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // reload
    //
    if (n.name () == "reload" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< reload_type > r (
        reload_traits::create (i, f, this));

      if (!reload_.present ())
      {
        this->reload_.set (r);
        continue;
      }
    }

    // envwarnsys
    //
    if (n.name () == "envwarnsys" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< envwarnsys_type > r (
        envwarnsys_traits::create (i, f, this));

      if (!envwarnsys_.present ())
      {
        this->envwarnsys_.set (r);
        continue;
      }
    }

    // use
    //
    if (n.name () == "use" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< use_type > r (
        use_traits::create (i, f, this));

      if (!use_.present ())
      {
        this->use_.set (r);
        continue;
      }
    }

    break;
  }

  if (!reload_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "reload",
      "");
  }

  if (!envwarnsys_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "envwarnsys",
      "");
  }

  if (!use_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "use",
      "");
  }
}

action_t* action_t::
_clone (::xml_schema::flags f,
        ::xml_schema::container* c) const
{
  return new class action_t (*this, f, c);
}

action_t::
~action_t ()
{
}

// movement_t
//

movement_t::
movement_t (const forward_type& forward,
            const backward_type& backward,
            const right_type& right,
            const left_type& left,
            const sprint_type& sprint,
            const jump_type& jump)
: ::xml_schema::type (),
  forward_ (forward, ::xml_schema::flags (), this),
  backward_ (backward, ::xml_schema::flags (), this),
  right_ (right, ::xml_schema::flags (), this),
  left_ (left, ::xml_schema::flags (), this),
  sprint_ (sprint, ::xml_schema::flags (), this),
  jump_ (jump, ::xml_schema::flags (), this)
{
}

movement_t::
movement_t (const movement_t& x,
            ::xml_schema::flags f,
            ::xml_schema::container* c)
: ::xml_schema::type (x, f, c),
  forward_ (x.forward_, f, this),
  backward_ (x.backward_, f, this),
  right_ (x.right_, f, this),
  left_ (x.left_, f, this),
  sprint_ (x.sprint_, f, this),
  jump_ (x.jump_, f, this)
{
}

movement_t::
movement_t (const ::xercesc::DOMElement& e,
            ::xml_schema::flags f,
            ::xml_schema::container* c)
: ::xml_schema::type (e, f | ::xml_schema::flags::base, c),
  forward_ (f, this),
  backward_ (f, this),
  right_ (f, this),
  left_ (f, this),
  sprint_ (f, this),
  jump_ (f, this)
{
  if ((f & ::xml_schema::flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false);
    this->parse (p, f);
  }
}

void movement_t::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::flags f)
{
  for (; p.more_elements (); p.next_element ())
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // forward
    //
    if (n.name () == "forward" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< forward_type > r (
        forward_traits::create (i, f, this));

      if (!forward_.present ())
      {
        this->forward_.set (r);
        continue;
      }
    }

    // backward
    //
    if (n.name () == "backward" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< backward_type > r (
        backward_traits::create (i, f, this));

      if (!backward_.present ())
      {
        this->backward_.set (r);
        continue;
      }
    }

    // right
    //
    if (n.name () == "right" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< right_type > r (
        right_traits::create (i, f, this));

      if (!right_.present ())
      {
        this->right_.set (r);
        continue;
      }
    }

    // left
    //
    if (n.name () == "left" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< left_type > r (
        left_traits::create (i, f, this));

      if (!left_.present ())
      {
        this->left_.set (r);
        continue;
      }
    }

    // sprint
    //
    if (n.name () == "sprint" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< sprint_type > r (
        sprint_traits::create (i, f, this));

      if (!sprint_.present ())
      {
        this->sprint_.set (r);
        continue;
      }
    }

    // jump
    //
    if (n.name () == "jump" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< jump_type > r (
        jump_traits::create (i, f, this));

      if (!jump_.present ())
      {
        this->jump_.set (r);
        continue;
      }
    }

    break;
  }

  if (!forward_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "forward",
      "");
  }

  if (!backward_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "backward",
      "");
  }

  if (!right_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "right",
      "");
  }

  if (!left_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "left",
      "");
  }

  if (!sprint_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "sprint",
      "");
  }

  if (!jump_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "jump",
      "");
  }
}

movement_t* movement_t::
_clone (::xml_schema::flags f,
        ::xml_schema::container* c) const
{
  return new class movement_t (*this, f, c);
}

movement_t::
~movement_t ()
{
}

// volume_t
//

volume_t::
volume_t (const character_type& character,
          const music_type& music,
          const soundfx_type& soundfx)
: ::xml_schema::type (),
  character_ (character, ::xml_schema::flags (), this),
  music_ (music, ::xml_schema::flags (), this),
  soundfx_ (soundfx, ::xml_schema::flags (), this)
{
}

volume_t::
volume_t (const volume_t& x,
          ::xml_schema::flags f,
          ::xml_schema::container* c)
: ::xml_schema::type (x, f, c),
  character_ (x.character_, f, this),
  music_ (x.music_, f, this),
  soundfx_ (x.soundfx_, f, this)
{
}

volume_t::
volume_t (const ::xercesc::DOMElement& e,
          ::xml_schema::flags f,
          ::xml_schema::container* c)
: ::xml_schema::type (e, f | ::xml_schema::flags::base, c),
  character_ (f, this),
  music_ (f, this),
  soundfx_ (f, this)
{
  if ((f & ::xml_schema::flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false);
    this->parse (p, f);
  }
}

void volume_t::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::flags f)
{
  for (; p.more_elements (); p.next_element ())
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // character
    //
    if (n.name () == "character" && n.namespace_ ().empty ())
    {
      if (!character_.present ())
      {
        this->character_.set (character_traits::create (i, f, this));
        continue;
      }
    }

    // music
    //
    if (n.name () == "music" && n.namespace_ ().empty ())
    {
      if (!music_.present ())
      {
        this->music_.set (music_traits::create (i, f, this));
        continue;
      }
    }

    // soundfx
    //
    if (n.name () == "soundfx" && n.namespace_ ().empty ())
    {
      if (!soundfx_.present ())
      {
        this->soundfx_.set (soundfx_traits::create (i, f, this));
        continue;
      }
    }

    break;
  }

  if (!character_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "character",
      "");
  }

  if (!music_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "music",
      "");
  }

  if (!soundfx_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "soundfx",
      "");
  }
}

volume_t* volume_t::
_clone (::xml_schema::flags f,
        ::xml_schema::container* c) const
{
  return new class volume_t (*this, f, c);
}

volume_t::
~volume_t ()
{
}

// configuration_t
//

configuration_t::
configuration_t (const action_type& action,
                 const movement_type& movement,
                 const volume_type& volume)
: ::xml_schema::type (),
  action_ (action, ::xml_schema::flags (), this),
  movement_ (movement, ::xml_schema::flags (), this),
  volume_ (volume, ::xml_schema::flags (), this)
{
}

configuration_t::
configuration_t (::std::auto_ptr< action_type >& action,
                 ::std::auto_ptr< movement_type >& movement,
                 ::std::auto_ptr< volume_type >& volume)
: ::xml_schema::type (),
  action_ (action, ::xml_schema::flags (), this),
  movement_ (movement, ::xml_schema::flags (), this),
  volume_ (volume, ::xml_schema::flags (), this)
{
}

configuration_t::
configuration_t (const configuration_t& x,
                 ::xml_schema::flags f,
                 ::xml_schema::container* c)
: ::xml_schema::type (x, f, c),
  action_ (x.action_, f, this),
  movement_ (x.movement_, f, this),
  volume_ (x.volume_, f, this)
{
}

configuration_t::
configuration_t (const ::xercesc::DOMElement& e,
                 ::xml_schema::flags f,
                 ::xml_schema::container* c)
: ::xml_schema::type (e, f | ::xml_schema::flags::base, c),
  action_ (f, this),
  movement_ (f, this),
  volume_ (f, this)
{
  if ((f & ::xml_schema::flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false);
    this->parse (p, f);
  }
}

void configuration_t::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::flags f)
{
  for (; p.more_elements (); p.next_element ())
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // action
    //
    if (n.name () == "action" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< action_type > r (
        action_traits::create (i, f, this));

      if (!action_.present ())
      {
        this->action_.set (r);
        continue;
      }
    }

    // movement
    //
    if (n.name () == "movement" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< movement_type > r (
        movement_traits::create (i, f, this));

      if (!movement_.present ())
      {
        this->movement_.set (r);
        continue;
      }
    }

    // volume
    //
    if (n.name () == "volume" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< volume_type > r (
        volume_traits::create (i, f, this));

      if (!volume_.present ())
      {
        this->volume_.set (r);
        continue;
      }
    }

    break;
  }

  if (!action_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "action",
      "");
  }

  if (!movement_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "movement",
      "");
  }

  if (!volume_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "volume",
      "");
  }
}

configuration_t* configuration_t::
_clone (::xml_schema::flags f,
        ::xml_schema::container* c) const
{
  return new class configuration_t (*this, f, c);
}

configuration_t::
~configuration_t ()
{
}

#include <istream>
#include <xsd/cxx/xml/sax/std-input-source.hxx>
#include <xsd/cxx/tree/error-handler.hxx>

::std::auto_ptr< ::configuration_t >
configuration (const ::std::string& u,
               ::xml_schema::flags f,
               const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::tree::error_handler< char > h;

  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      u, h, p, f));

  h.throw_if_failed< ::xsd::cxx::tree::parsing< char > > ();

  ::std::auto_ptr< ::configuration_t > r (
    ::configuration (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::configuration_t >
configuration (const ::std::string& u,
               ::xml_schema::error_handler& h,
               ::xml_schema::flags f,
               const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      u, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  ::std::auto_ptr< ::configuration_t > r (
    ::configuration (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::configuration_t >
configuration (const ::std::string& u,
               ::xercesc::DOMErrorHandler& h,
               ::xml_schema::flags f,
               const ::xml_schema::properties& p)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      u, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  ::std::auto_ptr< ::configuration_t > r (
    ::configuration (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::configuration_t >
configuration (::std::istream& is,
               ::xml_schema::flags f,
               const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::configuration (isrc, f, p);
}

::std::auto_ptr< ::configuration_t >
configuration (::std::istream& is,
               ::xml_schema::error_handler& h,
               ::xml_schema::flags f,
               const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::configuration (isrc, h, f, p);
}

::std::auto_ptr< ::configuration_t >
configuration (::std::istream& is,
               ::xercesc::DOMErrorHandler& h,
               ::xml_schema::flags f,
               const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::configuration (isrc, h, f, p);
}

::std::auto_ptr< ::configuration_t >
configuration (::std::istream& is,
               const ::std::string& sid,
               ::xml_schema::flags f,
               const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::configuration (isrc, f, p);
}

::std::auto_ptr< ::configuration_t >
configuration (::std::istream& is,
               const ::std::string& sid,
               ::xml_schema::error_handler& h,
               ::xml_schema::flags f,
               const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::configuration (isrc, h, f, p);
}

::std::auto_ptr< ::configuration_t >
configuration (::std::istream& is,
               const ::std::string& sid,
               ::xercesc::DOMErrorHandler& h,
               ::xml_schema::flags f,
               const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::configuration (isrc, h, f, p);
}

::std::auto_ptr< ::configuration_t >
configuration (::xercesc::InputSource& i,
               ::xml_schema::flags f,
               const ::xml_schema::properties& p)
{
  ::xsd::cxx::tree::error_handler< char > h;

  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  h.throw_if_failed< ::xsd::cxx::tree::parsing< char > > ();

  ::std::auto_ptr< ::configuration_t > r (
    ::configuration (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::configuration_t >
configuration (::xercesc::InputSource& i,
               ::xml_schema::error_handler& h,
               ::xml_schema::flags f,
               const ::xml_schema::properties& p)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  ::std::auto_ptr< ::configuration_t > r (
    ::configuration (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::configuration_t >
configuration (::xercesc::InputSource& i,
               ::xercesc::DOMErrorHandler& h,
               ::xml_schema::flags f,
               const ::xml_schema::properties& p)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  ::std::auto_ptr< ::configuration_t > r (
    ::configuration (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::configuration_t >
configuration (const ::xercesc::DOMDocument& d,
               ::xml_schema::flags f,
               const ::xml_schema::properties& p)
{
  if (f & ::xml_schema::flags::keep_dom)
  {
    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > c (
      static_cast< ::xercesc::DOMDocument* > (d.cloneNode (true)));

    ::std::auto_ptr< ::configuration_t > r (
      ::configuration (
        c, f | ::xml_schema::flags::own_dom, p));

    return r;
  }

  const ::xercesc::DOMElement& e (*d.getDocumentElement ());
  const ::xsd::cxx::xml::qualified_name< char > n (
    ::xsd::cxx::xml::dom::name< char > (e));

  if (n.name () == "configuration" &&
      n.namespace_ () == "")
  {
    ::std::auto_ptr< ::configuration_t > r (
      ::xsd::cxx::tree::traits< ::configuration_t, char >::create (
        e, f, 0));
    return r;
  }

  throw ::xsd::cxx::tree::unexpected_element < char > (
    n.name (),
    n.namespace_ (),
    "configuration",
    "");
}

::std::auto_ptr< ::configuration_t >
configuration (::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument >& d,
               ::xml_schema::flags f,
               const ::xml_schema::properties&)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > c (
    ((f & ::xml_schema::flags::keep_dom) &&
     !(f & ::xml_schema::flags::own_dom))
    ? static_cast< ::xercesc::DOMDocument* > (d->cloneNode (true))
    : 0);

  ::xercesc::DOMDocument& doc (c.get () ? *c : *d);
  const ::xercesc::DOMElement& e (*doc.getDocumentElement ());

  const ::xsd::cxx::xml::qualified_name< char > n (
    ::xsd::cxx::xml::dom::name< char > (e));

  if (f & ::xml_schema::flags::keep_dom)
    doc.setUserData (::xml_schema::dom::tree_node_key,
                     (c.get () ? &c : &d),
                     0);

  if (n.name () == "configuration" &&
      n.namespace_ () == "")
  {
    ::std::auto_ptr< ::configuration_t > r (
      ::xsd::cxx::tree::traits< ::configuration_t, char >::create (
        e, f, 0));
    return r;
  }

  throw ::xsd::cxx::tree::unexpected_element < char > (
    n.name (),
    n.namespace_ (),
    "configuration",
    "");
}

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

