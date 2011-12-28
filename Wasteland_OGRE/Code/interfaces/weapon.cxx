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

#include "weapon.hxx"

// sounds_t
// 

const sounds_t::fire_type& sounds_t::
fire () const
{
  return this->fire_.get ();
}

sounds_t::fire_type& sounds_t::
fire ()
{
  return this->fire_.get ();
}

void sounds_t::
fire (const fire_type& x)
{
  this->fire_.set (x);
}

void sounds_t::
fire (::std::auto_ptr< fire_type > x)
{
  this->fire_.set (x);
}

const sounds_t::reload_type& sounds_t::
reload () const
{
  return this->reload_.get ();
}

sounds_t::reload_type& sounds_t::
reload ()
{
  return this->reload_.get ();
}

void sounds_t::
reload (const reload_type& x)
{
  this->reload_.set (x);
}

void sounds_t::
reload (::std::auto_ptr< reload_type > x)
{
  this->reload_.set (x);
}

const sounds_t::putaway_type& sounds_t::
putaway () const
{
  return this->putaway_.get ();
}

sounds_t::putaway_type& sounds_t::
putaway ()
{
  return this->putaway_.get ();
}

void sounds_t::
putaway (const putaway_type& x)
{
  this->putaway_.set (x);
}

void sounds_t::
putaway (::std::auto_ptr< putaway_type > x)
{
  this->putaway_.set (x);
}

const sounds_t::dryfire_type& sounds_t::
dryfire () const
{
  return this->dryfire_.get ();
}

sounds_t::dryfire_type& sounds_t::
dryfire ()
{
  return this->dryfire_.get ();
}

void sounds_t::
dryfire (const dryfire_type& x)
{
  this->dryfire_.set (x);
}

void sounds_t::
dryfire (::std::auto_ptr< dryfire_type > x)
{
  this->dryfire_.set (x);
}

const sounds_t::altreload_type& sounds_t::
altreload () const
{
  return this->altreload_.get ();
}

sounds_t::altreload_type& sounds_t::
altreload ()
{
  return this->altreload_.get ();
}

void sounds_t::
altreload (const altreload_type& x)
{
  this->altreload_.set (x);
}

void sounds_t::
altreload (::std::auto_ptr< altreload_type > x)
{
  this->altreload_.set (x);
}

const sounds_t::altfire_type& sounds_t::
altfire () const
{
  return this->altfire_.get ();
}

sounds_t::altfire_type& sounds_t::
altfire ()
{
  return this->altfire_.get ();
}

void sounds_t::
altfire (const altfire_type& x)
{
  this->altfire_.set (x);
}

void sounds_t::
altfire (::std::auto_ptr< altfire_type > x)
{
  this->altfire_.set (x);
}


// soundFrame_t
// 

const soundFrame_t::frame_type& soundFrame_t::
frame () const
{
  return this->frame_.get ();
}

soundFrame_t::frame_type& soundFrame_t::
frame ()
{
  return this->frame_.get ();
}

void soundFrame_t::
frame (const frame_type& x)
{
  this->frame_.set (x);
}

const soundFrame_t::sound_type& soundFrame_t::
sound () const
{
  return this->sound_.get ();
}

soundFrame_t::sound_type& soundFrame_t::
sound ()
{
  return this->sound_.get ();
}

void soundFrame_t::
sound (const sound_type& x)
{
  this->sound_.set (x);
}

void soundFrame_t::
sound (::std::auto_ptr< sound_type > x)
{
  this->sound_.set (x);
}


// soundFrames_t
// 

const soundFrames_t::soundframe_sequence& soundFrames_t::
soundframe () const
{
  return this->soundframe_;
}

soundFrames_t::soundframe_sequence& soundFrames_t::
soundframe ()
{
  return this->soundframe_;
}

void soundFrames_t::
soundframe (const soundframe_sequence& s)
{
  this->soundframe_ = s;
}


// gameplay_t
// 

const gameplay_t::accuracy_type& gameplay_t::
accuracy () const
{
  return this->accuracy_.get ();
}

gameplay_t::accuracy_type& gameplay_t::
accuracy ()
{
  return this->accuracy_.get ();
}

void gameplay_t::
accuracy (const accuracy_type& x)
{
  this->accuracy_.set (x);
}

const gameplay_t::firerate_type& gameplay_t::
firerate () const
{
  return this->firerate_.get ();
}

gameplay_t::firerate_type& gameplay_t::
firerate ()
{
  return this->firerate_.get ();
}

void gameplay_t::
firerate (const firerate_type& x)
{
  this->firerate_.set (x);
}

const gameplay_t::range_type& gameplay_t::
range () const
{
  return this->range_.get ();
}

gameplay_t::range_type& gameplay_t::
range ()
{
  return this->range_.get ();
}

void gameplay_t::
range (const range_type& x)
{
  this->range_.set (x);
}

const gameplay_t::reloadQty_type& gameplay_t::
reloadQty () const
{
  return this->reloadQty_.get ();
}

gameplay_t::reloadQty_type& gameplay_t::
reloadQty ()
{
  return this->reloadQty_.get ();
}

void gameplay_t::
reloadQty (const reloadQty_type& x)
{
  this->reloadQty_.set (x);
}

const gameplay_t::damage_type& gameplay_t::
damage () const
{
  return this->damage_.get ();
}

gameplay_t::damage_type& gameplay_t::
damage ()
{
  return this->damage_.get ();
}

void gameplay_t::
damage (const damage_type& x)
{
  this->damage_.set (x);
}


// weapon_t
// 

const weapon_t::name_type& weapon_t::
name () const
{
  return this->name_.get ();
}

weapon_t::name_type& weapon_t::
name ()
{
  return this->name_.get ();
}

void weapon_t::
name (const name_type& x)
{
  this->name_.set (x);
}

void weapon_t::
name (::std::auto_ptr< name_type > x)
{
  this->name_.set (x);
}

const weapon_t::type_type& weapon_t::
type () const
{
  return this->type_.get ();
}

weapon_t::type_type& weapon_t::
type ()
{
  return this->type_.get ();
}

void weapon_t::
type (const type_type& x)
{
  this->type_.set (x);
}

void weapon_t::
type (::std::auto_ptr< type_type > x)
{
  this->type_.set (x);
}

const weapon_t::materialFile_type& weapon_t::
materialFile () const
{
  return this->materialFile_.get ();
}

weapon_t::materialFile_type& weapon_t::
materialFile ()
{
  return this->materialFile_.get ();
}

void weapon_t::
materialFile (const materialFile_type& x)
{
  this->materialFile_.set (x);
}

void weapon_t::
materialFile (::std::auto_ptr< materialFile_type > x)
{
  this->materialFile_.set (x);
}

const weapon_t::sounds_type& weapon_t::
sounds () const
{
  return this->sounds_.get ();
}

weapon_t::sounds_type& weapon_t::
sounds ()
{
  return this->sounds_.get ();
}

void weapon_t::
sounds (const sounds_type& x)
{
  this->sounds_.set (x);
}

void weapon_t::
sounds (::std::auto_ptr< sounds_type > x)
{
  this->sounds_.set (x);
}

const weapon_t::soundFrames_type& weapon_t::
soundFrames () const
{
  return this->soundFrames_.get ();
}

weapon_t::soundFrames_type& weapon_t::
soundFrames ()
{
  return this->soundFrames_.get ();
}

void weapon_t::
soundFrames (const soundFrames_type& x)
{
  this->soundFrames_.set (x);
}

void weapon_t::
soundFrames (::std::auto_ptr< soundFrames_type > x)
{
  this->soundFrames_.set (x);
}

const weapon_t::gameplay_type& weapon_t::
gameplay () const
{
  return this->gameplay_.get ();
}

weapon_t::gameplay_type& weapon_t::
gameplay ()
{
  return this->gameplay_.get ();
}

void weapon_t::
gameplay (const gameplay_type& x)
{
  this->gameplay_.set (x);
}

void weapon_t::
gameplay (::std::auto_ptr< gameplay_type > x)
{
  this->gameplay_.set (x);
}


#include <xsd/cxx/xml/dom/parsing-source.hxx>

// sounds_t
//

sounds_t::
sounds_t (const fire_type& fire,
          const reload_type& reload,
          const putaway_type& putaway,
          const dryfire_type& dryfire,
          const altreload_type& altreload,
          const altfire_type& altfire)
: ::xml_schema::type (),
  fire_ (fire, ::xml_schema::flags (), this),
  reload_ (reload, ::xml_schema::flags (), this),
  putaway_ (putaway, ::xml_schema::flags (), this),
  dryfire_ (dryfire, ::xml_schema::flags (), this),
  altreload_ (altreload, ::xml_schema::flags (), this),
  altfire_ (altfire, ::xml_schema::flags (), this)
{
}

sounds_t::
sounds_t (const sounds_t& x,
          ::xml_schema::flags f,
          ::xml_schema::container* c)
: ::xml_schema::type (x, f, c),
  fire_ (x.fire_, f, this),
  reload_ (x.reload_, f, this),
  putaway_ (x.putaway_, f, this),
  dryfire_ (x.dryfire_, f, this),
  altreload_ (x.altreload_, f, this),
  altfire_ (x.altfire_, f, this)
{
}

sounds_t::
sounds_t (const ::xercesc::DOMElement& e,
          ::xml_schema::flags f,
          ::xml_schema::container* c)
: ::xml_schema::type (e, f | ::xml_schema::flags::base, c),
  fire_ (f, this),
  reload_ (f, this),
  putaway_ (f, this),
  dryfire_ (f, this),
  altreload_ (f, this),
  altfire_ (f, this)
{
  if ((f & ::xml_schema::flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false);
    this->parse (p, f);
  }
}

void sounds_t::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::flags f)
{
  for (; p.more_elements (); p.next_element ())
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // fire
    //
    if (n.name () == "fire" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< fire_type > r (
        fire_traits::create (i, f, this));

      if (!fire_.present ())
      {
        this->fire_.set (r);
        continue;
      }
    }

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

    // putaway
    //
    if (n.name () == "putaway" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< putaway_type > r (
        putaway_traits::create (i, f, this));

      if (!putaway_.present ())
      {
        this->putaway_.set (r);
        continue;
      }
    }

    // dryfire
    //
    if (n.name () == "dryfire" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< dryfire_type > r (
        dryfire_traits::create (i, f, this));

      if (!dryfire_.present ())
      {
        this->dryfire_.set (r);
        continue;
      }
    }

    // altreload
    //
    if (n.name () == "altreload" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< altreload_type > r (
        altreload_traits::create (i, f, this));

      if (!altreload_.present ())
      {
        this->altreload_.set (r);
        continue;
      }
    }

    // altfire
    //
    if (n.name () == "altfire" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< altfire_type > r (
        altfire_traits::create (i, f, this));

      if (!altfire_.present ())
      {
        this->altfire_.set (r);
        continue;
      }
    }

    break;
  }

  if (!fire_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "fire",
      "");
  }

  if (!reload_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "reload",
      "");
  }

  if (!putaway_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "putaway",
      "");
  }

  if (!dryfire_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "dryfire",
      "");
  }

  if (!altreload_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "altreload",
      "");
  }

  if (!altfire_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "altfire",
      "");
  }
}

sounds_t* sounds_t::
_clone (::xml_schema::flags f,
        ::xml_schema::container* c) const
{
  return new class sounds_t (*this, f, c);
}

sounds_t::
~sounds_t ()
{
}

// soundFrame_t
//

soundFrame_t::
soundFrame_t (const frame_type& frame,
              const sound_type& sound)
: ::xml_schema::type (),
  frame_ (frame, ::xml_schema::flags (), this),
  sound_ (sound, ::xml_schema::flags (), this)
{
}

soundFrame_t::
soundFrame_t (const soundFrame_t& x,
              ::xml_schema::flags f,
              ::xml_schema::container* c)
: ::xml_schema::type (x, f, c),
  frame_ (x.frame_, f, this),
  sound_ (x.sound_, f, this)
{
}

soundFrame_t::
soundFrame_t (const ::xercesc::DOMElement& e,
              ::xml_schema::flags f,
              ::xml_schema::container* c)
: ::xml_schema::type (e, f | ::xml_schema::flags::base, c),
  frame_ (f, this),
  sound_ (f, this)
{
  if ((f & ::xml_schema::flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false);
    this->parse (p, f);
  }
}

void soundFrame_t::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::flags f)
{
  for (; p.more_elements (); p.next_element ())
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // frame
    //
    if (n.name () == "frame" && n.namespace_ ().empty ())
    {
      if (!frame_.present ())
      {
        this->frame_.set (frame_traits::create (i, f, this));
        continue;
      }
    }

    // sound
    //
    if (n.name () == "sound" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< sound_type > r (
        sound_traits::create (i, f, this));

      if (!sound_.present ())
      {
        this->sound_.set (r);
        continue;
      }
    }

    break;
  }

  if (!frame_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "frame",
      "");
  }

  if (!sound_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "sound",
      "");
  }
}

soundFrame_t* soundFrame_t::
_clone (::xml_schema::flags f,
        ::xml_schema::container* c) const
{
  return new class soundFrame_t (*this, f, c);
}

soundFrame_t::
~soundFrame_t ()
{
}

// soundFrames_t
//

soundFrames_t::
soundFrames_t ()
: ::xml_schema::type (),
  soundframe_ (::xml_schema::flags (), this)
{
}

soundFrames_t::
soundFrames_t (const soundFrames_t& x,
               ::xml_schema::flags f,
               ::xml_schema::container* c)
: ::xml_schema::type (x, f, c),
  soundframe_ (x.soundframe_, f, this)
{
}

soundFrames_t::
soundFrames_t (const ::xercesc::DOMElement& e,
               ::xml_schema::flags f,
               ::xml_schema::container* c)
: ::xml_schema::type (e, f | ::xml_schema::flags::base, c),
  soundframe_ (f, this)
{
  if ((f & ::xml_schema::flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false);
    this->parse (p, f);
  }
}

void soundFrames_t::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::flags f)
{
  for (; p.more_elements (); p.next_element ())
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // soundframe
    //
    if (n.name () == "soundframe" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< soundframe_type > r (
        soundframe_traits::create (i, f, this));

      this->soundframe_.push_back (r);
      continue;
    }

    break;
  }
}

soundFrames_t* soundFrames_t::
_clone (::xml_schema::flags f,
        ::xml_schema::container* c) const
{
  return new class soundFrames_t (*this, f, c);
}

soundFrames_t::
~soundFrames_t ()
{
}

// gameplay_t
//

gameplay_t::
gameplay_t (const accuracy_type& accuracy,
            const firerate_type& firerate,
            const range_type& range,
            const reloadQty_type& reloadQty,
            const damage_type& damage)
: ::xml_schema::type (),
  accuracy_ (accuracy, ::xml_schema::flags (), this),
  firerate_ (firerate, ::xml_schema::flags (), this),
  range_ (range, ::xml_schema::flags (), this),
  reloadQty_ (reloadQty, ::xml_schema::flags (), this),
  damage_ (damage, ::xml_schema::flags (), this)
{
}

gameplay_t::
gameplay_t (const gameplay_t& x,
            ::xml_schema::flags f,
            ::xml_schema::container* c)
: ::xml_schema::type (x, f, c),
  accuracy_ (x.accuracy_, f, this),
  firerate_ (x.firerate_, f, this),
  range_ (x.range_, f, this),
  reloadQty_ (x.reloadQty_, f, this),
  damage_ (x.damage_, f, this)
{
}

gameplay_t::
gameplay_t (const ::xercesc::DOMElement& e,
            ::xml_schema::flags f,
            ::xml_schema::container* c)
: ::xml_schema::type (e, f | ::xml_schema::flags::base, c),
  accuracy_ (f, this),
  firerate_ (f, this),
  range_ (f, this),
  reloadQty_ (f, this),
  damage_ (f, this)
{
  if ((f & ::xml_schema::flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false);
    this->parse (p, f);
  }
}

void gameplay_t::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::flags f)
{
  for (; p.more_elements (); p.next_element ())
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // accuracy
    //
    if (n.name () == "accuracy" && n.namespace_ ().empty ())
    {
      if (!accuracy_.present ())
      {
        this->accuracy_.set (accuracy_traits::create (i, f, this));
        continue;
      }
    }

    // firerate
    //
    if (n.name () == "firerate" && n.namespace_ ().empty ())
    {
      if (!firerate_.present ())
      {
        this->firerate_.set (firerate_traits::create (i, f, this));
        continue;
      }
    }

    // range
    //
    if (n.name () == "range" && n.namespace_ ().empty ())
    {
      if (!range_.present ())
      {
        this->range_.set (range_traits::create (i, f, this));
        continue;
      }
    }

    // reloadQty
    //
    if (n.name () == "reloadQty" && n.namespace_ ().empty ())
    {
      if (!reloadQty_.present ())
      {
        this->reloadQty_.set (reloadQty_traits::create (i, f, this));
        continue;
      }
    }

    // damage
    //
    if (n.name () == "damage" && n.namespace_ ().empty ())
    {
      if (!damage_.present ())
      {
        this->damage_.set (damage_traits::create (i, f, this));
        continue;
      }
    }

    break;
  }

  if (!accuracy_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "accuracy",
      "");
  }

  if (!firerate_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "firerate",
      "");
  }

  if (!range_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "range",
      "");
  }

  if (!reloadQty_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "reloadQty",
      "");
  }

  if (!damage_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "damage",
      "");
  }
}

gameplay_t* gameplay_t::
_clone (::xml_schema::flags f,
        ::xml_schema::container* c) const
{
  return new class gameplay_t (*this, f, c);
}

gameplay_t::
~gameplay_t ()
{
}

// weapon_t
//

weapon_t::
weapon_t (const name_type& name,
          const type_type& type,
          const materialFile_type& materialFile,
          const sounds_type& sounds,
          const soundFrames_type& soundFrames,
          const gameplay_type& gameplay)
: ::xml_schema::type (),
  name_ (name, ::xml_schema::flags (), this),
  type_ (type, ::xml_schema::flags (), this),
  materialFile_ (materialFile, ::xml_schema::flags (), this),
  sounds_ (sounds, ::xml_schema::flags (), this),
  soundFrames_ (soundFrames, ::xml_schema::flags (), this),
  gameplay_ (gameplay, ::xml_schema::flags (), this)
{
}

weapon_t::
weapon_t (const name_type& name,
          const type_type& type,
          const materialFile_type& materialFile,
          ::std::auto_ptr< sounds_type >& sounds,
          ::std::auto_ptr< soundFrames_type >& soundFrames,
          ::std::auto_ptr< gameplay_type >& gameplay)
: ::xml_schema::type (),
  name_ (name, ::xml_schema::flags (), this),
  type_ (type, ::xml_schema::flags (), this),
  materialFile_ (materialFile, ::xml_schema::flags (), this),
  sounds_ (sounds, ::xml_schema::flags (), this),
  soundFrames_ (soundFrames, ::xml_schema::flags (), this),
  gameplay_ (gameplay, ::xml_schema::flags (), this)
{
}

weapon_t::
weapon_t (const weapon_t& x,
          ::xml_schema::flags f,
          ::xml_schema::container* c)
: ::xml_schema::type (x, f, c),
  name_ (x.name_, f, this),
  type_ (x.type_, f, this),
  materialFile_ (x.materialFile_, f, this),
  sounds_ (x.sounds_, f, this),
  soundFrames_ (x.soundFrames_, f, this),
  gameplay_ (x.gameplay_, f, this)
{
}

weapon_t::
weapon_t (const ::xercesc::DOMElement& e,
          ::xml_schema::flags f,
          ::xml_schema::container* c)
: ::xml_schema::type (e, f | ::xml_schema::flags::base, c),
  name_ (f, this),
  type_ (f, this),
  materialFile_ (f, this),
  sounds_ (f, this),
  soundFrames_ (f, this),
  gameplay_ (f, this)
{
  if ((f & ::xml_schema::flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false);
    this->parse (p, f);
  }
}

void weapon_t::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::flags f)
{
  for (; p.more_elements (); p.next_element ())
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // name
    //
    if (n.name () == "name" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< name_type > r (
        name_traits::create (i, f, this));

      if (!name_.present ())
      {
        this->name_.set (r);
        continue;
      }
    }

    // type
    //
    if (n.name () == "type" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< type_type > r (
        type_traits::create (i, f, this));

      if (!type_.present ())
      {
        this->type_.set (r);
        continue;
      }
    }

    // materialFile
    //
    if (n.name () == "materialFile" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< materialFile_type > r (
        materialFile_traits::create (i, f, this));

      if (!materialFile_.present ())
      {
        this->materialFile_.set (r);
        continue;
      }
    }

    // sounds
    //
    if (n.name () == "sounds" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< sounds_type > r (
        sounds_traits::create (i, f, this));

      if (!sounds_.present ())
      {
        this->sounds_.set (r);
        continue;
      }
    }

    // soundFrames
    //
    if (n.name () == "soundFrames" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< soundFrames_type > r (
        soundFrames_traits::create (i, f, this));

      if (!soundFrames_.present ())
      {
        this->soundFrames_.set (r);
        continue;
      }
    }

    // gameplay
    //
    if (n.name () == "gameplay" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< gameplay_type > r (
        gameplay_traits::create (i, f, this));

      if (!gameplay_.present ())
      {
        this->gameplay_.set (r);
        continue;
      }
    }

    break;
  }

  if (!name_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "name",
      "");
  }

  if (!type_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "type",
      "");
  }

  if (!materialFile_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "materialFile",
      "");
  }

  if (!sounds_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "sounds",
      "");
  }

  if (!soundFrames_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "soundFrames",
      "");
  }

  if (!gameplay_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "gameplay",
      "");
  }
}

weapon_t* weapon_t::
_clone (::xml_schema::flags f,
        ::xml_schema::container* c) const
{
  return new class weapon_t (*this, f, c);
}

weapon_t::
~weapon_t ()
{
}

#include <istream>
#include <xsd/cxx/xml/sax/std-input-source.hxx>
#include <xsd/cxx/tree/error-handler.hxx>

::std::auto_ptr< ::weapon_t >
weapon (const ::std::string& u,
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

  ::std::auto_ptr< ::weapon_t > r (
    ::weapon (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::weapon_t >
weapon (const ::std::string& u,
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

  ::std::auto_ptr< ::weapon_t > r (
    ::weapon (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::weapon_t >
weapon (const ::std::string& u,
        ::xercesc::DOMErrorHandler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      u, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  ::std::auto_ptr< ::weapon_t > r (
    ::weapon (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::weapon_t >
weapon (::std::istream& is,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::weapon (isrc, f, p);
}

::std::auto_ptr< ::weapon_t >
weapon (::std::istream& is,
        ::xml_schema::error_handler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::weapon (isrc, h, f, p);
}

::std::auto_ptr< ::weapon_t >
weapon (::std::istream& is,
        ::xercesc::DOMErrorHandler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::weapon (isrc, h, f, p);
}

::std::auto_ptr< ::weapon_t >
weapon (::std::istream& is,
        const ::std::string& sid,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::weapon (isrc, f, p);
}

::std::auto_ptr< ::weapon_t >
weapon (::std::istream& is,
        const ::std::string& sid,
        ::xml_schema::error_handler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::weapon (isrc, h, f, p);
}

::std::auto_ptr< ::weapon_t >
weapon (::std::istream& is,
        const ::std::string& sid,
        ::xercesc::DOMErrorHandler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::weapon (isrc, h, f, p);
}

::std::auto_ptr< ::weapon_t >
weapon (::xercesc::InputSource& i,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xsd::cxx::tree::error_handler< char > h;

  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  h.throw_if_failed< ::xsd::cxx::tree::parsing< char > > ();

  ::std::auto_ptr< ::weapon_t > r (
    ::weapon (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::weapon_t >
weapon (::xercesc::InputSource& i,
        ::xml_schema::error_handler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  ::std::auto_ptr< ::weapon_t > r (
    ::weapon (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::weapon_t >
weapon (::xercesc::InputSource& i,
        ::xercesc::DOMErrorHandler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  ::std::auto_ptr< ::weapon_t > r (
    ::weapon (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::weapon_t >
weapon (const ::xercesc::DOMDocument& d,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  if (f & ::xml_schema::flags::keep_dom)
  {
    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > c (
      static_cast< ::xercesc::DOMDocument* > (d.cloneNode (true)));

    ::std::auto_ptr< ::weapon_t > r (
      ::weapon (
        c, f | ::xml_schema::flags::own_dom, p));

    return r;
  }

  const ::xercesc::DOMElement& e (*d.getDocumentElement ());
  const ::xsd::cxx::xml::qualified_name< char > n (
    ::xsd::cxx::xml::dom::name< char > (e));

  if (n.name () == "weapon" &&
      n.namespace_ () == "")
  {
    ::std::auto_ptr< ::weapon_t > r (
      ::xsd::cxx::tree::traits< ::weapon_t, char >::create (
        e, f, 0));
    return r;
  }

  throw ::xsd::cxx::tree::unexpected_element < char > (
    n.name (),
    n.namespace_ (),
    "weapon",
    "");
}

::std::auto_ptr< ::weapon_t >
weapon (::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument >& d,
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

  if (n.name () == "weapon" &&
      n.namespace_ () == "")
  {
    ::std::auto_ptr< ::weapon_t > r (
      ::xsd::cxx::tree::traits< ::weapon_t, char >::create (
        e, f, 0));
    return r;
  }

  throw ::xsd::cxx::tree::unexpected_element < char > (
    n.name (),
    n.namespace_ (),
    "weapon",
    "");
}

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

