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

#include "object.hxx"

// object_t
// 

const object_t::name_type& object_t::
name () const
{
  return this->name_.get ();
}

object_t::name_type& object_t::
name ()
{
  return this->name_.get ();
}

void object_t::
name (const name_type& x)
{
  this->name_.set (x);
}

void object_t::
name (::std::auto_ptr< name_type > x)
{
  this->name_.set (x);
}

const object_t::childName_type& object_t::
childName () const
{
  return this->childName_.get ();
}

object_t::childName_type& object_t::
childName ()
{
  return this->childName_.get ();
}

void object_t::
childName (const childName_type& x)
{
  this->childName_.set (x);
}

void object_t::
childName (::std::auto_ptr< childName_type > x)
{
  this->childName_.set (x);
}

const object_t::type_type& object_t::
type () const
{
  return this->type_.get ();
}

object_t::type_type& object_t::
type ()
{
  return this->type_.get ();
}

void object_t::
type (const type_type& x)
{
  this->type_.set (x);
}

void object_t::
type (::std::auto_ptr< type_type > x)
{
  this->type_.set (x);
}

const object_t::fileName_type& object_t::
fileName () const
{
  return this->fileName_.get ();
}

object_t::fileName_type& object_t::
fileName ()
{
  return this->fileName_.get ();
}

void object_t::
fileName (const fileName_type& x)
{
  this->fileName_.set (x);
}

void object_t::
fileName (::std::auto_ptr< fileName_type > x)
{
  this->fileName_.set (x);
}

const object_t::resGroup_type& object_t::
resGroup () const
{
  return this->resGroup_.get ();
}

object_t::resGroup_type& object_t::
resGroup ()
{
  return this->resGroup_.get ();
}

void object_t::
resGroup (const resGroup_type& x)
{
  this->resGroup_.set (x);
}

void object_t::
resGroup (::std::auto_ptr< resGroup_type > x)
{
  this->resGroup_.set (x);
}

const object_t::shadows_type& object_t::
shadows () const
{
  return this->shadows_.get ();
}

object_t::shadows_type& object_t::
shadows ()
{
  return this->shadows_.get ();
}

void object_t::
shadows (const shadows_type& x)
{
  this->shadows_.set (x);
}

const object_t::mass_type& object_t::
mass () const
{
  return this->mass_.get ();
}

object_t::mass_type& object_t::
mass ()
{
  return this->mass_.get ();
}

void object_t::
mass (const mass_type& x)
{
  this->mass_.set (x);
}

const object_t::collisionShape_type& object_t::
collisionShape () const
{
  return this->collisionShape_.get ();
}

object_t::collisionShape_type& object_t::
collisionShape ()
{
  return this->collisionShape_.get ();
}

void object_t::
collisionShape (const collisionShape_type& x)
{
  this->collisionShape_.set (x);
}

void object_t::
collisionShape (::std::auto_ptr< collisionShape_type > x)
{
  this->collisionShape_.set (x);
}

const object_t::colBoxHeight_type& object_t::
colBoxHeight () const
{
  return this->colBoxHeight_.get ();
}

object_t::colBoxHeight_type& object_t::
colBoxHeight ()
{
  return this->colBoxHeight_.get ();
}

void object_t::
colBoxHeight (const colBoxHeight_type& x)
{
  this->colBoxHeight_.set (x);
}

const object_t::colBoxWidth_type& object_t::
colBoxWidth () const
{
  return this->colBoxWidth_.get ();
}

object_t::colBoxWidth_type& object_t::
colBoxWidth ()
{
  return this->colBoxWidth_.get ();
}

void object_t::
colBoxWidth (const colBoxWidth_type& x)
{
  this->colBoxWidth_.set (x);
}

const object_t::colBoxDepth_type& object_t::
colBoxDepth () const
{
  return this->colBoxDepth_.get ();
}

object_t::colBoxDepth_type& object_t::
colBoxDepth ()
{
  return this->colBoxDepth_.get ();
}

void object_t::
colBoxDepth (const colBoxDepth_type& x)
{
  this->colBoxDepth_.set (x);
}

const object_t::colCapsuleHeight_type& object_t::
colCapsuleHeight () const
{
  return this->colCapsuleHeight_.get ();
}

object_t::colCapsuleHeight_type& object_t::
colCapsuleHeight ()
{
  return this->colCapsuleHeight_.get ();
}

void object_t::
colCapsuleHeight (const colCapsuleHeight_type& x)
{
  this->colCapsuleHeight_.set (x);
}

const object_t::colCapsuleWidth_type& object_t::
colCapsuleWidth () const
{
  return this->colCapsuleWidth_.get ();
}

object_t::colCapsuleWidth_type& object_t::
colCapsuleWidth ()
{
  return this->colCapsuleWidth_.get ();
}

void object_t::
colCapsuleWidth (const colCapsuleWidth_type& x)
{
  this->colCapsuleWidth_.set (x);
}

const object_t::colCubeSize_type& object_t::
colCubeSize () const
{
  return this->colCubeSize_.get ();
}

object_t::colCubeSize_type& object_t::
colCubeSize ()
{
  return this->colCubeSize_.get ();
}

void object_t::
colCubeSize (const colCubeSize_type& x)
{
  this->colCubeSize_.set (x);
}

const object_t::colCustomFile_type& object_t::
colCustomFile () const
{
  return this->colCustomFile_.get ();
}

object_t::colCustomFile_type& object_t::
colCustomFile ()
{
  return this->colCustomFile_.get ();
}

void object_t::
colCustomFile (const colCustomFile_type& x)
{
  this->colCustomFile_.set (x);
}

void object_t::
colCustomFile (::std::auto_ptr< colCustomFile_type > x)
{
  this->colCustomFile_.set (x);
}

const object_t::colSphereRadius_type& object_t::
colSphereRadius () const
{
  return this->colSphereRadius_.get ();
}

object_t::colSphereRadius_type& object_t::
colSphereRadius ()
{
  return this->colSphereRadius_.get ();
}

void object_t::
colSphereRadius (const colSphereRadius_type& x)
{
  this->colSphereRadius_.set (x);
}

const object_t::positionX_type& object_t::
positionX () const
{
  return this->positionX_.get ();
}

object_t::positionX_type& object_t::
positionX ()
{
  return this->positionX_.get ();
}

void object_t::
positionX (const positionX_type& x)
{
  this->positionX_.set (x);
}

const object_t::positionY_type& object_t::
positionY () const
{
  return this->positionY_.get ();
}

object_t::positionY_type& object_t::
positionY ()
{
  return this->positionY_.get ();
}

void object_t::
positionY (const positionY_type& x)
{
  this->positionY_.set (x);
}

const object_t::positionZ_type& object_t::
positionZ () const
{
  return this->positionZ_.get ();
}

object_t::positionZ_type& object_t::
positionZ ()
{
  return this->positionZ_.get ();
}

void object_t::
positionZ (const positionZ_type& x)
{
  this->positionZ_.set (x);
}

const object_t::pointX_type& object_t::
pointX () const
{
  return this->pointX_.get ();
}

object_t::pointX_type& object_t::
pointX ()
{
  return this->pointX_.get ();
}

void object_t::
pointX (const pointX_type& x)
{
  this->pointX_.set (x);
}

const object_t::pointY_type& object_t::
pointY () const
{
  return this->pointY_.get ();
}

object_t::pointY_type& object_t::
pointY ()
{
  return this->pointY_.get ();
}

void object_t::
pointY (const pointY_type& x)
{
  this->pointY_.set (x);
}

const object_t::pointZ_type& object_t::
pointZ () const
{
  return this->pointZ_.get ();
}

object_t::pointZ_type& object_t::
pointZ ()
{
  return this->pointZ_.get ();
}

void object_t::
pointZ (const pointZ_type& x)
{
  this->pointZ_.set (x);
}


#include <xsd/cxx/xml/dom/parsing-source.hxx>

// object_t
//

object_t::
object_t (const name_type& name,
          const childName_type& childName,
          const type_type& type,
          const fileName_type& fileName,
          const resGroup_type& resGroup,
          const shadows_type& shadows,
          const mass_type& mass,
          const collisionShape_type& collisionShape,
          const colBoxHeight_type& colBoxHeight,
          const colBoxWidth_type& colBoxWidth,
          const colBoxDepth_type& colBoxDepth,
          const colCapsuleHeight_type& colCapsuleHeight,
          const colCapsuleWidth_type& colCapsuleWidth,
          const colCubeSize_type& colCubeSize,
          const colCustomFile_type& colCustomFile,
          const colSphereRadius_type& colSphereRadius,
          const positionX_type& positionX,
          const positionY_type& positionY,
          const positionZ_type& positionZ,
          const pointX_type& pointX,
          const pointY_type& pointY,
          const pointZ_type& pointZ)
: ::xml_schema::type (),
  name_ (name, ::xml_schema::flags (), this),
  childName_ (childName, ::xml_schema::flags (), this),
  type_ (type, ::xml_schema::flags (), this),
  fileName_ (fileName, ::xml_schema::flags (), this),
  resGroup_ (resGroup, ::xml_schema::flags (), this),
  shadows_ (shadows, ::xml_schema::flags (), this),
  mass_ (mass, ::xml_schema::flags (), this),
  collisionShape_ (collisionShape, ::xml_schema::flags (), this),
  colBoxHeight_ (colBoxHeight, ::xml_schema::flags (), this),
  colBoxWidth_ (colBoxWidth, ::xml_schema::flags (), this),
  colBoxDepth_ (colBoxDepth, ::xml_schema::flags (), this),
  colCapsuleHeight_ (colCapsuleHeight, ::xml_schema::flags (), this),
  colCapsuleWidth_ (colCapsuleWidth, ::xml_schema::flags (), this),
  colCubeSize_ (colCubeSize, ::xml_schema::flags (), this),
  colCustomFile_ (colCustomFile, ::xml_schema::flags (), this),
  colSphereRadius_ (colSphereRadius, ::xml_schema::flags (), this),
  positionX_ (positionX, ::xml_schema::flags (), this),
  positionY_ (positionY, ::xml_schema::flags (), this),
  positionZ_ (positionZ, ::xml_schema::flags (), this),
  pointX_ (pointX, ::xml_schema::flags (), this),
  pointY_ (pointY, ::xml_schema::flags (), this),
  pointZ_ (pointZ, ::xml_schema::flags (), this)
{
}

object_t::
object_t (const object_t& x,
          ::xml_schema::flags f,
          ::xml_schema::container* c)
: ::xml_schema::type (x, f, c),
  name_ (x.name_, f, this),
  childName_ (x.childName_, f, this),
  type_ (x.type_, f, this),
  fileName_ (x.fileName_, f, this),
  resGroup_ (x.resGroup_, f, this),
  shadows_ (x.shadows_, f, this),
  mass_ (x.mass_, f, this),
  collisionShape_ (x.collisionShape_, f, this),
  colBoxHeight_ (x.colBoxHeight_, f, this),
  colBoxWidth_ (x.colBoxWidth_, f, this),
  colBoxDepth_ (x.colBoxDepth_, f, this),
  colCapsuleHeight_ (x.colCapsuleHeight_, f, this),
  colCapsuleWidth_ (x.colCapsuleWidth_, f, this),
  colCubeSize_ (x.colCubeSize_, f, this),
  colCustomFile_ (x.colCustomFile_, f, this),
  colSphereRadius_ (x.colSphereRadius_, f, this),
  positionX_ (x.positionX_, f, this),
  positionY_ (x.positionY_, f, this),
  positionZ_ (x.positionZ_, f, this),
  pointX_ (x.pointX_, f, this),
  pointY_ (x.pointY_, f, this),
  pointZ_ (x.pointZ_, f, this)
{
}

object_t::
object_t (const ::xercesc::DOMElement& e,
          ::xml_schema::flags f,
          ::xml_schema::container* c)
: ::xml_schema::type (e, f | ::xml_schema::flags::base, c),
  name_ (f, this),
  childName_ (f, this),
  type_ (f, this),
  fileName_ (f, this),
  resGroup_ (f, this),
  shadows_ (f, this),
  mass_ (f, this),
  collisionShape_ (f, this),
  colBoxHeight_ (f, this),
  colBoxWidth_ (f, this),
  colBoxDepth_ (f, this),
  colCapsuleHeight_ (f, this),
  colCapsuleWidth_ (f, this),
  colCubeSize_ (f, this),
  colCustomFile_ (f, this),
  colSphereRadius_ (f, this),
  positionX_ (f, this),
  positionY_ (f, this),
  positionZ_ (f, this),
  pointX_ (f, this),
  pointY_ (f, this),
  pointZ_ (f, this)
{
  if ((f & ::xml_schema::flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false);
    this->parse (p, f);
  }
}

void object_t::
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

    // childName
    //
    if (n.name () == "childName" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< childName_type > r (
        childName_traits::create (i, f, this));

      if (!childName_.present ())
      {
        this->childName_.set (r);
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

    // fileName
    //
    if (n.name () == "fileName" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< fileName_type > r (
        fileName_traits::create (i, f, this));

      if (!fileName_.present ())
      {
        this->fileName_.set (r);
        continue;
      }
    }

    // resGroup
    //
    if (n.name () == "resGroup" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< resGroup_type > r (
        resGroup_traits::create (i, f, this));

      if (!resGroup_.present ())
      {
        this->resGroup_.set (r);
        continue;
      }
    }

    // shadows
    //
    if (n.name () == "shadows" && n.namespace_ ().empty ())
    {
      if (!shadows_.present ())
      {
        this->shadows_.set (shadows_traits::create (i, f, this));
        continue;
      }
    }

    // mass
    //
    if (n.name () == "mass" && n.namespace_ ().empty ())
    {
      if (!mass_.present ())
      {
        this->mass_.set (mass_traits::create (i, f, this));
        continue;
      }
    }

    // collisionShape
    //
    if (n.name () == "collisionShape" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< collisionShape_type > r (
        collisionShape_traits::create (i, f, this));

      if (!collisionShape_.present ())
      {
        this->collisionShape_.set (r);
        continue;
      }
    }

    // colBoxHeight
    //
    if (n.name () == "colBoxHeight" && n.namespace_ ().empty ())
    {
      if (!colBoxHeight_.present ())
      {
        this->colBoxHeight_.set (colBoxHeight_traits::create (i, f, this));
        continue;
      }
    }

    // colBoxWidth
    //
    if (n.name () == "colBoxWidth" && n.namespace_ ().empty ())
    {
      if (!colBoxWidth_.present ())
      {
        this->colBoxWidth_.set (colBoxWidth_traits::create (i, f, this));
        continue;
      }
    }

    // colBoxDepth
    //
    if (n.name () == "colBoxDepth" && n.namespace_ ().empty ())
    {
      if (!colBoxDepth_.present ())
      {
        this->colBoxDepth_.set (colBoxDepth_traits::create (i, f, this));
        continue;
      }
    }

    // colCapsuleHeight
    //
    if (n.name () == "colCapsuleHeight" && n.namespace_ ().empty ())
    {
      if (!colCapsuleHeight_.present ())
      {
        this->colCapsuleHeight_.set (colCapsuleHeight_traits::create (i, f, this));
        continue;
      }
    }

    // colCapsuleWidth
    //
    if (n.name () == "colCapsuleWidth" && n.namespace_ ().empty ())
    {
      if (!colCapsuleWidth_.present ())
      {
        this->colCapsuleWidth_.set (colCapsuleWidth_traits::create (i, f, this));
        continue;
      }
    }

    // colCubeSize
    //
    if (n.name () == "colCubeSize" && n.namespace_ ().empty ())
    {
      if (!colCubeSize_.present ())
      {
        this->colCubeSize_.set (colCubeSize_traits::create (i, f, this));
        continue;
      }
    }

    // colCustomFile
    //
    if (n.name () == "colCustomFile" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< colCustomFile_type > r (
        colCustomFile_traits::create (i, f, this));

      if (!colCustomFile_.present ())
      {
        this->colCustomFile_.set (r);
        continue;
      }
    }

    // colSphereRadius
    //
    if (n.name () == "colSphereRadius" && n.namespace_ ().empty ())
    {
      if (!colSphereRadius_.present ())
      {
        this->colSphereRadius_.set (colSphereRadius_traits::create (i, f, this));
        continue;
      }
    }

    // positionX
    //
    if (n.name () == "positionX" && n.namespace_ ().empty ())
    {
      if (!positionX_.present ())
      {
        this->positionX_.set (positionX_traits::create (i, f, this));
        continue;
      }
    }

    // positionY
    //
    if (n.name () == "positionY" && n.namespace_ ().empty ())
    {
      if (!positionY_.present ())
      {
        this->positionY_.set (positionY_traits::create (i, f, this));
        continue;
      }
    }

    // positionZ
    //
    if (n.name () == "positionZ" && n.namespace_ ().empty ())
    {
      if (!positionZ_.present ())
      {
        this->positionZ_.set (positionZ_traits::create (i, f, this));
        continue;
      }
    }

    // pointX
    //
    if (n.name () == "pointX" && n.namespace_ ().empty ())
    {
      if (!pointX_.present ())
      {
        this->pointX_.set (pointX_traits::create (i, f, this));
        continue;
      }
    }

    // pointY
    //
    if (n.name () == "pointY" && n.namespace_ ().empty ())
    {
      if (!pointY_.present ())
      {
        this->pointY_.set (pointY_traits::create (i, f, this));
        continue;
      }
    }

    // pointZ
    //
    if (n.name () == "pointZ" && n.namespace_ ().empty ())
    {
      if (!pointZ_.present ())
      {
        this->pointZ_.set (pointZ_traits::create (i, f, this));
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

  if (!childName_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "childName",
      "");
  }

  if (!type_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "type",
      "");
  }

  if (!fileName_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "fileName",
      "");
  }

  if (!resGroup_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "resGroup",
      "");
  }

  if (!shadows_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "shadows",
      "");
  }

  if (!mass_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "mass",
      "");
  }

  if (!collisionShape_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "collisionShape",
      "");
  }

  if (!colBoxHeight_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "colBoxHeight",
      "");
  }

  if (!colBoxWidth_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "colBoxWidth",
      "");
  }

  if (!colBoxDepth_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "colBoxDepth",
      "");
  }

  if (!colCapsuleHeight_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "colCapsuleHeight",
      "");
  }

  if (!colCapsuleWidth_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "colCapsuleWidth",
      "");
  }

  if (!colCubeSize_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "colCubeSize",
      "");
  }

  if (!colCustomFile_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "colCustomFile",
      "");
  }

  if (!colSphereRadius_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "colSphereRadius",
      "");
  }

  if (!positionX_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "positionX",
      "");
  }

  if (!positionY_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "positionY",
      "");
  }

  if (!positionZ_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "positionZ",
      "");
  }

  if (!pointX_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "pointX",
      "");
  }

  if (!pointY_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "pointY",
      "");
  }

  if (!pointZ_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "pointZ",
      "");
  }
}

object_t* object_t::
_clone (::xml_schema::flags f,
        ::xml_schema::container* c) const
{
  return new class object_t (*this, f, c);
}

object_t::
~object_t ()
{
}

#include <istream>
#include <xsd/cxx/xml/sax/std-input-source.hxx>
#include <xsd/cxx/tree/error-handler.hxx>

::std::auto_ptr< ::object_t >
object (const ::std::string& u,
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

  ::std::auto_ptr< ::object_t > r (
    ::object (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::object_t >
object (const ::std::string& u,
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

  ::std::auto_ptr< ::object_t > r (
    ::object (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::object_t >
object (const ::std::string& u,
        ::xercesc::DOMErrorHandler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      u, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  ::std::auto_ptr< ::object_t > r (
    ::object (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::object_t >
object (::std::istream& is,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::object (isrc, f, p);
}

::std::auto_ptr< ::object_t >
object (::std::istream& is,
        ::xml_schema::error_handler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::object (isrc, h, f, p);
}

::std::auto_ptr< ::object_t >
object (::std::istream& is,
        ::xercesc::DOMErrorHandler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::object (isrc, h, f, p);
}

::std::auto_ptr< ::object_t >
object (::std::istream& is,
        const ::std::string& sid,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::object (isrc, f, p);
}

::std::auto_ptr< ::object_t >
object (::std::istream& is,
        const ::std::string& sid,
        ::xml_schema::error_handler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::flags::dont_initialize) == 0,
    (f & ::xml_schema::flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::object (isrc, h, f, p);
}

::std::auto_ptr< ::object_t >
object (::std::istream& is,
        const ::std::string& sid,
        ::xercesc::DOMErrorHandler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::object (isrc, h, f, p);
}

::std::auto_ptr< ::object_t >
object (::xercesc::InputSource& i,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xsd::cxx::tree::error_handler< char > h;

  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  h.throw_if_failed< ::xsd::cxx::tree::parsing< char > > ();

  ::std::auto_ptr< ::object_t > r (
    ::object (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::object_t >
object (::xercesc::InputSource& i,
        ::xml_schema::error_handler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  ::std::auto_ptr< ::object_t > r (
    ::object (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::object_t >
object (::xercesc::InputSource& i,
        ::xercesc::DOMErrorHandler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  ::std::auto_ptr< ::object_t > r (
    ::object (
      d, f | ::xml_schema::flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::object_t >
object (const ::xercesc::DOMDocument& d,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
{
  if (f & ::xml_schema::flags::keep_dom)
  {
    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > c (
      static_cast< ::xercesc::DOMDocument* > (d.cloneNode (true)));

    ::std::auto_ptr< ::object_t > r (
      ::object (
        c, f | ::xml_schema::flags::own_dom, p));

    return r;
  }

  const ::xercesc::DOMElement& e (*d.getDocumentElement ());
  const ::xsd::cxx::xml::qualified_name< char > n (
    ::xsd::cxx::xml::dom::name< char > (e));

  if (n.name () == "object" &&
      n.namespace_ () == "")
  {
    ::std::auto_ptr< ::object_t > r (
      ::xsd::cxx::tree::traits< ::object_t, char >::create (
        e, f, 0));
    return r;
  }

  throw ::xsd::cxx::tree::unexpected_element < char > (
    n.name (),
    n.namespace_ (),
    "object",
    "");
}

::std::auto_ptr< ::object_t >
object (::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument >& d,
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

  if (n.name () == "object" &&
      n.namespace_ () == "")
  {
    ::std::auto_ptr< ::object_t > r (
      ::xsd::cxx::tree::traits< ::object_t, char >::create (
        e, f, 0));
    return r;
  }

  throw ::xsd::cxx::tree::unexpected_element < char > (
    n.name (),
    n.namespace_ (),
    "object",
    "");
}

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

