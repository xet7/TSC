#include "editor_items_loader.h"
#include "../../objects/sprite.h"

using namespace SMC;
namespace fs = boost::filesystem;

cEditorItemsLoader::cEditorItemsLoader()
{
	//
}

cEditorItemsLoader::~cEditorItemsLoader()
{
	//
}

void cEditorItemsLoader::parse_file(fs::path filename)
{
	m_items_file = filename;
	xmlpp::SaxParser::parse_file(path_to_utf8(filename));
}

void cEditorItemsLoader::on_start_document()
{
	//
}

void cEditorItemsLoader::on_end_document()
{
	//
}

void cEditorItemsLoader::on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& properties)
{
	if (name == "property" || name == "Property") {
		std::string key;
		std::string value;

		/* Collect all the <property> elements for the surrounding
		 * mayor element (like <settings> or <sprite>). When the
		 * surrounding element is closed, the results are handled
		 * in on_end_element(). */
		for(xmlpp::SaxParser::AttributeList::const_iterator iter = properties.begin(); iter != properties.end(); iter++) {
			xmlpp::SaxParser::Attribute attr = *iter;

			if (attr.name == "name")
				key = attr.value;
			else if (attr.name == "value")
				value = attr.value;
		}

		m_current_properties[key] = value;
	}
}

void cEditorItemsLoader::on_end_element(const Glib::ustring& name)
{
	// <property> tags are parsed cumulatively in on_start_element()
	// so all have been collected when the surrounding element
	// terminates here.
	if (name == "property")
		return;

	std::string objname = m_current_properties["object_name"];
	std::string tags = m_current_properties["object_tags"];

	cSprite* p_sprite = get_object(objname, m_current_properties, level_engine_version);

	if (!p_sprite) {
		std::cerr << "Warning: Editor item could not be created: " << objname << std::endl;
		return;
	}

	p_sprite->m_editor_tags = tags.c_str();
	m_tagged_sprites.push_back(p_sprite);
}

// virtual
cSprite* cEditorItemsLoader::get_object(const Glib::ustring& name, XmlAttributes& attributes, int engine_version)
{
	throw(new NotImplementedError("cEditorItemsLoader::get_object() must be overridden in a subclass."));
	return NULL; // Not reached
}

vector<cSprite*> cEditorItemsLoader::get_tagged_sprites()
{
	return m_tagged_sprites;
}
