#include "level_loader.h"

namespace fs = boost::filesystem;
using namespace SMC;

cLevelLoader::cLevelLoader(fs::path levelfile)
	: xmlpp::SaxParser()
{
	m_levelfile = levelfile; // Copy
	mp_level    = NULL;
}

cLevelLoader::~cLevelLoader()
{
	// Do not delete the cLevel instance — it is used by the
	// caller and deleted by him.
	mp_level = NULL;
}

cLevel* cLevelLoader::Get_Level()
{
	return mp_level;
}

/***************************************
 * SAX parser callbacks
 ***************************************/

void cLevelLoader::on_start_document()
{
	if (mp_level)
		throw("Restarted XML parser after already starting it."); // FIXME: proper exception

	mp_level = new cLevel();
}

void cLevelLoader::on_end_document()
{
	// Nothing
}

void cLevelLoader::on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& properties)
{

}

void cLevelLoader::on_end_element(const Glib::ustring& name)
{

}

void cLevelLoader::on_characters(const Glib::ustring& text)
{

}

/***************************************
 * Parsers for mayor XML tags
 ***************************************/

