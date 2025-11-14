#ifndef PRODUCTS_MAIN_H
#define PRODUCTS_MAIN_H

#include <iostream>
#include <string>
#include <stack>
using namespace std;

class Reviews {};   // placeholder

class Products {
private:
    int id;
    string name;
    string category;
    double price;
    int stockQuantity;
    string brand;
    string model;
    string color;
    string dimensions;
    double weight;
    string material;
    string warranty;
    string releaseDate;
    string originCountry;
    string batteryLife;
    double screenSize;
    string processor;
    int ramGB;
    int storageGB;
    string connectivity;
    string features;

    // ---- NEW IMAGE FIELDS ----
    string mainImageURL;
    // string thumbnailURL;
    // string galleryImage1;
    // string galleryImage2;
    // string galleryImage3;

    stack<Reviews> reviews;

public:

    // ---- FULL CONSTRUCTOR WITH IMAGES ----
    Products(int id,
             string name,
             string category,
             double price,
             int stockQuantity,
             string brand,
             string model,
             string color,
             string dimensions,
             double weight,
             string material,
             string warranty,
             string releaseDate,
             string originCountry,
             string batteryLife,
             double screenSize,
             string processor,
             int ramGB,
             int storageGB,
             string connectivity,
             string features,
             string mainImageURL
            //  string thumbnailURL,
            //  string galleryImage1,
            //  string galleryImage2,
            //  string galleryImage3
            )
    {
        this->id = id;
        this->name = name;
        this->category = category;
        this->price = price;
        this->stockQuantity = stockQuantity;
        this->brand = brand;
        this->model = model;
        this->color = color;
        this->dimensions = dimensions;
        this->weight = weight;
        this->material = material;
        this->warranty = warranty;
        this->releaseDate = releaseDate;
        this->originCountry = originCountry;
        this->batteryLife = batteryLife;
        this->screenSize = screenSize;
        this->processor = processor;
        this->ramGB = ramGB;
        this->storageGB = storageGB;
        this->connectivity = connectivity;
        this->features = features;

        // new image fields
        this->mainImageURL = mainImageURL;
        // this->thumbnailURL = thumbnailURL;
        // this->galleryImage1 = galleryImage1;
        // this->galleryImage2 = galleryImage2;
        // this->galleryImage3 = galleryImage3;
    }

    // DEFAULT CONSTRUCTOR
    Products() {
        id = 0;
        price = 0;
        stockQuantity = 0;
        weight = 0;
        screenSize = 0;
        ramGB = 0;
        storageGB = 0;

        name = category = brand = model = color = dimensions =
        material = warranty = releaseDate = originCountry =
        batteryLife = processor = connectivity = features = "";

        mainImageURL ="";
        // thumbnailURL = galleryImage1 =
        //galleryImage2 = galleryImage3 = "";
    }

    // COPY CONSTRUCTOR
    Products(const Products& other) {
        id = other.id;
        name = other.name;
        category = other.category;
        price = other.price;
        stockQuantity = other.stockQuantity;
        brand = other.brand;
        model = other.model;
        color = other.color;
        dimensions = other.dimensions;
        weight = other.weight;
        material = other.material;
        warranty = other.warranty;
        releaseDate = other.releaseDate;
        originCountry = other.originCountry;
        batteryLife = other.batteryLife;
        screenSize = other.screenSize;
        processor = other.processor;
        ramGB = other.ramGB;
        storageGB = other.storageGB;
        connectivity = other.connectivity;
        features = other.features;

        mainImageURL = other.mainImageURL;
        // thumbnailURL = other.thumbnailURL;
        // galleryImage1 = other.galleryImage1;
        // galleryImage2 = other.galleryImage2;
        // galleryImage3 = other.galleryImage3;

        reviews = other.reviews;
    }

    // ASSIGNMENT OPERATOR
    Products& operator=(const Products& other) {
        if (this != &other) {
            id = other.id;
            name = other.name;
            category = other.category;
            price = other.price;
            stockQuantity = other.stockQuantity;
            brand = other.brand;
            model = other.model;
            color = other.color;
            dimensions = other.dimensions;
            weight = other.weight;
            material = other.material;
            warranty = other.warranty;
            releaseDate = other.releaseDate;
            originCountry = other.originCountry;
            batteryLife = other.batteryLife;
            screenSize = other.screenSize;
            processor = other.processor;
            ramGB = other.ramGB;
            storageGB = other.storageGB;
            connectivity = other.connectivity;
            features = other.features;

            mainImageURL = other.mainImageURL;
            // thumbnailURL = other.thumbnailURL;
            // galleryImage1 = other.galleryImage1;
            // galleryImage2 = other.galleryImage2;
            // galleryImage3 = other.galleryImage3;

            reviews = other.reviews;
        }
        return *this;
    }

    // ---- GETTERS ----
    int getID() const { return id; }
    string getname() const { return name; }
    string getcategory() const { return category; }
    double getprice() const { return price; }
    int getStock() const { return stockQuantity; }
    string getbrand() const { return brand; }
    string getmodel() const { return model; }
    string getcolor() const { return color; }
    string getdimensions() const { return dimensions; }
    double getweight() const { return weight; }
    string getmaterial() const { return material; }
    string getwarranty() const { return warranty; }
    string getreleaseDate() const { return releaseDate; }
    string getorigin() const { return originCountry; }
    string getbattery() const { return batteryLife; }
    double getscreensize() const { return screenSize; }
    string getprocessor() const { return processor; }
    int getram() const { return ramGB; }
    int getstorage() const { return storageGB; }
    string getconnectivity() const { return connectivity; }
    string getfeatures() const { return features; }

    // ---- IMAGE GETTERS ----
    string getMainImage() const { return mainImageURL; }
    // string getThumbnail() const { return thumbnailURL; }
    // string getGallery1() const { return galleryImage1; }
    // string getGallery2() const { return galleryImage2; }
    // string getGallery3() const { return galleryImage3; }

    // ---- SETTERS ----
    void setprice(double p) { price = p; }
    void setStock(int q) { stockQuantity = q; }
    void setcolor(string c) { color = c; }

    void setMainImage(string url) { mainImageURL = url; }
    // void setThumbnail(string url) { thumbnailURL = url; }
    // void setGallery1(string url) { galleryImage1 = url; }
    // void setGallery2(string url) { galleryImage2 = url; }
    // void setGallery3(string url) { galleryImage3 = url; }

    ~Products() {}
};

#endif
