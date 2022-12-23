#pragma once
#include "../RadarData.h"

#include <vector>
#include <map>
#include <string>

// defines a radar product
class RadarProduct {
public:
	enum ProductType{
		// base radar volume products loaded from files
		PRODUCT_BASE,
		// derived radar volume products created from other products
		// use deriveVolume to get the derived volume
		PRODUCT_DERIVED_VOLUME,
		// derived arbatrary data products created from other products
		PRODUCT_DERIVED_DATA,
	};
	
	
	// the type of volume that this product is
	RadarData::VolumeType volumeType = RadarData::VOLUME_UNKNOWN;
	
	// the type of product that this product is
	ProductType productType = PRODUCT_DERIVED_VOLUME;
	
	// full descriptive name of radar product
	std::string name = "Missing Name";
	
	// short abbreviated name of radar product
	std::string shortName = "MISSING";
	
	// the products that a derived product relyes on
	std::vector<RadarData::VolumeType> dependencies = {};
	
	// derive the product from a map containing its dependencies
	// this should not be called if the dependencies are not fulfilled or this is not a derived product
	// inputProducts should contain all dependencies in decompressed form
	virtual RadarData* deriveVolume(std::map<RadarData::VolumeType, RadarData*> inputProducts);
	
	// list of all volume products
	static std::map<RadarData::VolumeType, RadarProduct*> products;
	
	static RadarProduct* GetProduct(RadarData::VolumeType type){
		if(products.find(type) != products.end()){
			return products[type];
		}else{
			return NULL;
		}
	}
	
	virtual ~RadarProduct();
};

// class for base radar products
class RadarProductBase : public RadarProduct {
public:
	RadarProductBase(RadarData::VolumeType type, std::string productName, std::string shortProductName);
};