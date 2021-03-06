#include "platform/CCImage.h"

#include "platform/CCFileUtils.h"

#include <QImage>
#include <QFile>
#include <QBuffer>

namespace cocos2d
{

ssize_t CC_DLL qImageSize(const QImage &image)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
	return image.sizeInBytes();
#else
	return image.byteCount();
#endif	
}

bool Image::PNG_PREMULTIPLIED_ALPHA_ENABLED = false;

Image::Image()
	: _data(nullptr)
	, _dataLen(0)
	, _width(0)
	, _height(0)
	, _unpack(false)
	, _fileType(Format::UNKNOWN)
	, _pixelFormat(PixelFormat::NONE)
	, _numberOfMipmaps(0)
	, _hasPremultipliedAlpha(false)
{
	memset(&_mipmaps, 0, sizeof(_mipmaps));
}

Image::~Image()
{
	// destroy _image
}

bool Image::initWithImageFile(const std::string& path)
{
    auto filePath = FileUtils::getInstance()->fullPathForFilename(path);

    return initWithImageFileThreadSafe(filePath);
}

bool Image::initWithImageFileThreadSafe(const std::string& path)
{
	auto filePathQt = QString::fromStdString(path);
	QFile file(filePathQt);
	if (initWithDevice(&file)) {
		_filePath = path;
		return true;
	}
	
	return false;
}

bool Image::initWithImageData(const unsigned char * data, ssize_t dataLen)
{
	auto rawData = QByteArray::fromRawData(
		reinterpret_cast<const char*>(data), int(dataLen));
	QBuffer buffer;
	buffer.setData(rawData);
	return initWithDevice(&buffer);
}
	
bool Image::initWithDevice(QIODevice *device)
{
	if (!device->isOpen() && !device->open(QIODevice::ReadOnly)) {
		return false;
	}
	
	QImage image;
	if (!image.load(device, nullptr)) {
		return false;
	}
	
	bool hasAlpha = image.hasAlphaChannel();
	_hasPremultipliedAlpha = hasAlpha && 
		image.pixelFormat().premultiplied() == QPixelFormat::Premultiplied;

	if (_hasPremultipliedAlpha)
	{
		image = image.convertToFormat(QImage::Format_RGBA8888_Premultiplied);
		_pixelFormat = PixelFormat::RGBA8888;
	} else if (hasAlpha)
	{
		image = image.convertToFormat(QImage::Format_RGBA8888);
		_pixelFormat = PixelFormat::RGBA8888;
	} else
	{
		image = image.convertToFormat(QImage::Format_RGB888);
		_pixelFormat = PixelFormat::RGB888;
	}

	_image.reset(new QImage);
	*_image = image;
	_data = _image->bits();
	_width = quint16(image.width());
	_height = quint16(image.height());
	_dataLen = qImageSize(image);
	_numberOfMipmaps = 0;

	return true;
}

bool Image::saveToFile(const std::string &filename, bool isToRGB)
{
	if (nullptr == _image)
		return false;

	QImage image = *_image;

	if (isToRGB && image.format() != QImage::Format_RGB888)
		image = image.convertToFormat(QImage::Format_RGB888);

	return image.save(QString::fromStdString(filename));
}

void Image::premultiplyAlpha()
{
	if (!_image) {
		return;
	}
	
	if (_image->format() != QImage::Format_RGBA8888)
	{
		return;
	}
	
	*_image = _image->convertToFormat(QImage::Format_RGBA8888_Premultiplied);
	_hasPremultipliedAlpha = true;
	_data = _image->bits();
}

void Image::reversePremultipliedAlpha()
{
	if (!_image) {
		return;
	}
	
	if (_image->format() != QImage::Format_RGBA8888_Premultiplied)
	{
		return;
	}
	
	*_image = _image->convertToFormat(QImage::Format_RGBA8888);
	_hasPremultipliedAlpha = false;
	_data = _image->bits();
}

bool Image::initWithRawData(
		const unsigned char *data, ssize_t dataLen, 
		int width, int height, int bitsPerComponent, bool preMulti)
{
	Q_ASSERT(width <= 65535);
	Q_ASSERT(height <= 65535);

	QImage::Format sourceImageFormat;
	QImage::Format storeImageFormat = preMulti
		? QImage::Format_RGBA8888_Premultiplied
		: QImage::Format_RGB888;
	int length = width * height;
	Q_ASSERT(length > 0);

	_pixelFormat = preMulti ?
			PixelFormat::RGBA8888 : PixelFormat::RGB888;
	
	int bytesPerPixel = dataLen / length;

	switch (bytesPerPixel)
	{
		case 2:
			Q_ASSERT(bitsPerComponent == (preMulti ? 4 : 5));
			sourceImageFormat = preMulti
				? QImage::Format_ARGB4444_Premultiplied
				: QImage::Format_RGB555;
			break;

		case 3:
			Q_ASSERT(bitsPerComponent == (preMulti ? 5 : 8));
			sourceImageFormat = preMulti
				? QImage::Format_ARGB8555_Premultiplied
				: QImage::Format_RGB888;
			break;

		case 4:
			Q_ASSERT(bitsPerComponent == 8);
			sourceImageFormat = preMulti
				? QImage::Format_RGBA8888_Premultiplied
				: QImage::Format_RGBA8888;
			storeImageFormat = sourceImageFormat;
			_pixelFormat = PixelFormat::RGBA8888;
			break;

		default:
			Q_UNREACHABLE();
			return false;
	}

	_image.reset(new QImage(width, height, sourceImageFormat));

	int bytesPerLine = width * (_image->depth() / 8);

	auto source = data;

	for (int y = 0; y < height; y++)
	{
		memcpy(_image->scanLine(y), source, bytesPerLine);
		source += bytesPerLine;
	}

	if (sourceImageFormat != storeImageFormat)
		*_image = _image->convertToFormat(storeImageFormat);

	auto pixelFormat = _image->pixelFormat();
	bool hasAlpha = pixelFormat.alphaUsage() == QPixelFormat::UsesAlpha;
	_hasPremultipliedAlpha = hasAlpha 
			&& pixelFormat.premultiplied() == QPixelFormat::Premultiplied;

	_data = _image->bits();
	_width = quint16(_image->width());
	_height = quint16(_image->height());
	_dataLen = qImageSize(*_image);
	_numberOfMipmaps = 0;

	return true;
}

int Image::getBitPerPixel()
{
	return _image ? (_image->bytesPerLine() / _image->width()) * 8 : 0;
}

bool Image::hasAlpha()
{
	return _image ? _image->hasAlphaChannel() : false;
}

bool Image::isCompressed()
{
	return false;
}

void Image::setPVRImagesHavePremultipliedAlpha(bool)
{
}
}
