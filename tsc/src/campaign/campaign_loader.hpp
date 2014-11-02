#ifndef TSC_CAMPAIGN_LOADER_HPP
#define TSC_CAMPAIGN_LOADER_HPP
#include "../core/global_game.hpp"
#include "../core/xml_attributes.hpp"
#include "campaign_manager.hpp"

namespace TSC {

    class cCampaignLoader: public xmlpp::SaxParser {
    public:
        cCampaignLoader();
        virtual ~cCampaignLoader();

        // Parse the given filename. Use this function instead of
        // bare xmlpp’s parse_file() that accepts a Glib::ustring —
        // this function sets some internal members.
        virtual void parse_file(boost::filesystem::path filename);
        // After finishing parsing, contains a pointer to a cCampaign
        // instance. This pointer must be freed by you. Returns NULL
        // before parsing.
        cCampaign* Get_Campaign();

    protected: // SAX parser callbacks
        virtual void on_start_document();
        virtual void on_end_document();
        virtual void on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& properties);
        virtual void on_end_element(const Glib::ustring& name);

        void Handle_Information();
        void Handle_Target();

        cCampaign* mp_campaign;
        boost::filesystem::path m_campaignfile;
        XmlAttributes m_current_properties;
    };

}
#endif
