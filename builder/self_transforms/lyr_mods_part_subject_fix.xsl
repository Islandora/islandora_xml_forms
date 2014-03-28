<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:mods="http://www.loc.gov/mods/v3"
    version="1.0">
<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" media-type="text/xml"/>
<xsl:strip-space elements="*"/>
    
    <xsl:template match="mods:mods">
        <xsl:copy>
            <xsl:copy-of select="@*"/>
            <xsl:apply-templates select="mods:titleInfo"/>
            <xsl:apply-templates select="mods:name"/>
            <xsl:apply-templates select="mods:originInfo"/>
            <xsl:apply-templates select="mods:physicalDescription"/>
            <xsl:apply-templates select="mods:abstract"/>
            <xsl:apply-templates select="mods:tableOfContents"/>
            <xsl:apply-templates select="mods:targetAudience"/>
            <xsl:apply-templates select="mods:note"/>
            <xsl:apply-templates select="mods:genre"/>
            <xsl:apply-templates select="mods:typeOfResource"/>
            <xsl:apply-templates select="mods:relatedItem"/>
            <xsl:apply-templates select="mods:language"/>
            <xsl:for-each select="//mods:topic">
                <subject xmlns="http://www.loc.gov/mods/v3"><topic><xsl:value-of select="node()"/></topic></subject>
            </xsl:for-each>
            <xsl:for-each select="//mods:geographic">
                <subject xmlns="http://www.loc.gov/mods/v3"><geographic><xsl:value-of select="node()"/></geographic></subject>
            </xsl:for-each>
            <xsl:for-each select="//mods:temporal">
                <subject xmlns="http://www.loc.gov/mods/v3"><temporal><xsl:value-of select="node()"/></temporal></subject>
            </xsl:for-each>
            <xsl:apply-templates select="mods:classification"/>
            <xsl:apply-templates select="mods:identifier"/>
            <xsl:apply-templates select="mods:location"/>
            <xsl:apply-templates select="mods:accessCondition"/>
            <xsl:apply-templates select="mods:recordInfo"/>
            <xsl:apply-templates select="mods:extension"/>
        </xsl:copy>
    </xsl:template>
    
    <xsl:template match="/mods:mods/mods:part" />

    <xsl:template match="mods:name[mods:namePart[not(text())]]"/>
    
    <xsl:template match="*[not(node())]"/>
    
    <xsl:template match="*[string-length(self::*) = 0]"></xsl:template>
    
    <xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:for-each select="@*">
                <xsl:copy/>
            </xsl:for-each>
            <xsl:apply-templates select="node()[boolean(normalize-space())]|@*"/>
        </xsl:copy>
    </xsl:template>

    
</xsl:stylesheet>