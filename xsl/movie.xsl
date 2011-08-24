<?xml version="1.0"?>

<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/review">
	<html>
	<head>
	<basefont face="Arial" size="2"/>
	</head>
	<body>
	<xsl:apply-templates select="title"/> (<xsl:apply-templates
select="year"/>)
	<br />
	<xsl:apply-templates select="teaser"/>
	<p />
	<xsl:apply-templates select="cast"/>
	<br />
	<xsl:apply-templates select="director"/>
	<br />
	<xsl:apply-templates select="duration"/>
	<br />
	<xsl:apply-templates select="rating"/>
	<p>
	<xsl:apply-templates select="body"/>
	</p>
	</body>
	</html>
</xsl:template>

<xsl:template match="title">
<b><xsl:value-of select="." /></b>
</xsl:template>

<xsl:template match="teaser">
<xsl:value-of select="." />
</xsl:template>

<xsl:template match="director">
<b>Director: </b> <xsl:value-of select="." />
</xsl:template>

<xsl:template match="duration">
<b>Duration: </b> <xsl:value-of select="." /> minutes
</xsl:template>

<xsl:template match="rating">
<b>Our rating: </b> <xsl:value-of select="." />
</xsl:template>

<xsl:template match="cast">
<b>Cast: </b>
<xsl:apply-templates/>
</xsl:template>

<xsl:template match="person[position() != last()]">
<xsl:value-of select="." />,
</xsl:template>

<xsl:template match="person[position() = (last()-1)]">
<xsl:value-of select="." />
</xsl:template>

<xsl:template match="person[position() = last()]">
and <xsl:value-of select="." />
</xsl:template>

<xsl:template match="body">
	<xsl:apply-templates/>
</xsl:template>

<xsl:template match="body//title">
	<i><xsl:value-of select="." /></i>
</xsl:template>

<xsl:template match="body//person">
	<b><xsl:value-of select="." /></b>
</xsl:template>

</xsl:stylesheet>
