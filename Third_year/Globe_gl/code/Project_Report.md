# P3-Globe-GL-Vis

## Equip

Grup BO8

- Andres Rio Nogues        -- (AndresRioo)
- Francesc Navarro Vazquez -- (quico16)


## Un cop hem vist el codi ... 

- **Com es carrega la textura del globus terraqüi?**

Al final de tot el codi, quan definim el material via els shaders, on pasem el textureMap amb variables uniforms 

``` js
    // Set the world texture and start the animation
    Promise.all([
    new THREE.TextureLoader().loadAsync('./resources/earth-day.jpg')
    ]).then(([texture]) => {
    const material = new THREE.ShaderMaterial({
        uniforms: {
        textureMap: { value: texture },
        },
        vertexShader: basicShaders.vertexShader,
        fragmentShader: basicShaders.fragmentShader
    });
    world.globeMaterial(material);
    });
```


- **On es defineix la geometria definida per a representar els valors d'un punt?**


La geometria dels punts que representen els valors no es defineix directament per el codi. És la llibreria globe.gl la que s'encarrega de crear i col·locar els punts a partir de les dades que li passes. Però l’aspecte visual de cada punt sí que els controlem amb aquestes línies dins la funció mappingSelectedData:

``` js

 if ( method === 'points' ) {
    // Configure the world globe 
    world
    .pointAltitude((d) => (d.population - minWeight)/(maxWeight - minWeight))
    .pointColor(d => weightColor(d.population))
    .pointRadius(0.2)
    .pointLabel(getTooltip)
    .labelLat('lat')
    .labelLng('lng');

    // Set the data by year
    world.pointsData(dataObject[year]);

    // Set the legend 
    updateLegend(weightColor, minWeight, maxWeight);
} 

```

Aquest codi controla l'altura, el color, el radi i les etiquetes de cada punt. 

- **On es calcula el valor del color a cada punt?**

Abans ja hem vist que el color surt de la funció `weightColor`, que segons un minim i un maxim mapeja els colors amb la escala del arc de sant martí. 

``` js

const weightColor = scaleSequential(interpolateRainbow)
  .domain([minWeight, maxWeight])
  .clamp(false);

```

- **S’usa un shader per pintar les barres que representen les dades del fitxer JSON?**

No es defineix explícitament cap shader per pintar els punts que representen les dades del JSON. Però sí que globe.gl i Three.js fan servir shaders internament per renderitzar aquests punts en WebGL.

- **On s'inicialitzen les variables dels shaders?**

Algunes de les variables com la normalMatrix, la modelViewMatrix i la
projectionMatrix són matrius ja definides i controlades des de la pròpia llibreria de Three.js. Altres variables com el textureMap les inicialitzem nosaltres al crear el shaderMaterial com variables uniform. 

- **Quin shader es fa servir per pintar el globus?**

Fem servir el basic shader definit a `util.js`

``` js
// Custom shaders: Basic shader
const basicShaders = {
  vertexShader: `
    varying vec3 vNormal;
    varying vec2 vUv;
    void main() {
        vNormal = normalize(normalMatrix * normal);
        vUv = uv;
        gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
    }
  `,
  fragmentShader: `
    uniform sampler2D textureMap;
    varying vec3 vNormal;
    varying vec2 vUv;
    void main() {
        vec4 diffuse = texture2D(textureMap, vUv);
        gl_FragColor = diffuse;
    }
  `
};
```



## Prova a....

- **Canviar el color de visualització de les barres associades a cada dada**

Si canviem el point color a qualsevol valor podrem veure els punts amb diferent color. 

``` js

if ( method === 'points' ) {
    // Configure the world globe 
    world
    .pointAltitude((d) => (d.population - minWeight)/(maxWeight - minWeight))
    .pointColor(() => 'red')
    //.pointColor(d => weightColor(d.population))
    .pointRadius(0.2)
    .pointLabel(getTooltip)
    .labelLat('lat')
    .labelLng('lng');

    // Set the data by year
    world.pointsData(dataObject[year]);

    // Set the legend 
    updateLegend(weightColor, minWeight, maxWeight);
} 

```

![punts vermells](/IMGreadme/prova%20red.png)

- **Canviar l'alçada de la barra a la que es refereix cada dada**

Igual que abans pero amb l'altitud. 

``` js

if ( method === 'points' ) {
    // Configure the world globe 
    world
    .pointAltitude(() => 50)
    //.pointAltitude((d) => (d.population - minWeight)/(maxWeight - minWeight))
    //.pointColor(() => 'red')
    .pointColor(d => weightColor(d.population))
    .pointRadius(0.2)
    .pointLabel(getTooltip)
    .labelLat('lat')
    .labelLng('lng');
    
    // Set the data by year
    world.pointsData(dataObject[year]);

    // Set the legend 
    updateLegend(weightColor, minWeight, maxWeight);
} 

```

![punts amb mateixa altitud](/IMGreadme/prova%20altitud.png)




- **Canviar la textura del globus terraqüi. Pots agafar alguna de les textures utilitzades a la pràctica 2 o de la que trobis per internet.**

``` js

// Set the world texture and start the animation
Promise.all([
new THREE.TextureLoader().loadAsync('./resources/discoBall.jpeg')
]).then(([texture]) => {
const material = new THREE.ShaderMaterial({
    uniforms: {
    textureMap: { value: texture },
    },
    vertexShader: basicShaders.vertexShader,
    fragmentShader: basicShaders.fragmentShader
});
world.globeMaterial(material);
});


```

![diferent textura](/IMGreadme/prova%20discoBall.png)



- **Canvia el json per a que es visualitzin altres dades geolocalitzades. Cerca per Internet dades a visualitzar que tinguin informació geogràfica. Per exemple, a https://github.com/mwgg/Airports pots trobar el fitxer airports.dat amb dades dels aeroports del món, però cal que busquis d’altres dades per visualitzar.**

Per canviar de dades cal primer seleccionar el fitxer desitjat i cal canviar el mètode parseData per poder llegir el format corresponent a aquell fitxer -

``` js

// Index.html

let dataObject = await getDataFetch('./datasets/airports.json', parseData);
```


``` js

// UTILS.JS

async function parseData(data) {
  let dataObject = {};
  
  //... nou parseig de dades
}

```

# Pràctica 3 : Visualització d'unes dades del món

## 1. Busquis un conjunt de dades geolocalitzades. Poden ser temporals o no i poden tenir connexions entre elles o no. Per un mateix punt (una latitud i longitud) pots tenir un o varis valors (Escalars o vectorials).

El conjunt de dades `countries.json` consisteix en informació bàsica i pràctica sobre tots els països del món, amb dades geolocalitzades (latitud i longitud) i diverses variables relacionades amb identitat nacional, economia i zona horària. 

Cada país té una latitud i longitud assignada (punt sobre el globus).

Cada punt té diferents dades (valors escalars, per país):

- Informació política i administrativa: name, capital, region, subregion
- Informació econòmica: currency, currency_symbol
- Informació pràctica: phonecode, timezone, tld (domini web)
- Informació cultural: native (nom nadiu), translations (traduccions a múltiples idiomes)
- Identificadors internacionals: iso2, iso3, numeric_code

Cada entrada representa un país únic (punt geogràfic), alguns camps com timezones són llistes (pot tenir diversos valors vectorials per país), les dades no tenen temporalitat dinàmica, però inclouen informació horària (gmtOffset, tzName) que és rellevant pel temps local i les connexions entre països no són explícites, però es poden inferir per region, currency o timezones.

A més utilitzem un altre conjunt de dades per saber les fronteres entre els diferents països `area_country.geojson`. 


## 2. Exploris el tipus de dades que tens (What?): són categòriques, són ordinals, són quantitatives? Quina dimensió tenen? Tenen temporalitat? Estan interconnectades?

L’estructura del conjunt de dades es basa en entrades per país, cada una amb diversos atributs classificats de la següent manera: 

- Categòriques nominals : 
    - name, capital, currency, currency_symbol, tld, native, region, subregion, timezone.abbreviation, tzName
    - També phonecode, tot i ser numèric, s’interpreta com a categoria (no mesura).   

- Categòriques amb jerarquia (ordinal feble): 
    - Region i subregion poden entendre’s com a jerarquia geogràfica, però no tenen un ordre estricte.

- Quantitatives : 
    - latitude, longitude : valors decimals, posicionen el país. 
    - gmtOffset  : valor numèric que representa la diferència horària respecte UTC.

- Multivalorades
    - timezones és una llista d’objectes (pot haver-hi més d’una zona horària per país).

- Text multillenguatge:
    - translations: diccionari amb traduccions del nom del país.

Per la dimensió de les nostres dades tenim 1 registre per país i per cada país tenim uns 20 atributs. 

Sobre la dimensió espacial tenim les coordenades geogràfiques explícites (latitud i altitud) i sobre la dimensió temporal no tenim dades que evolucionin temporalment pero si informació de la zona horaria, que seria una forma de metadada temporal estàtica.

Les nostres dades a més no estan explícitament interconnectades, pero podem tenir relacions en els següents atributs:

- Mateixa regió / subregió
- Moneda compartida 
- Mateixa franja horaria
- Idioma

## 3. Defineixis l’objectiu de la teva visualització (Why?): analitzar (descobrir tendències, outliers), cerca de valors, comparar, resumir, etc. Defineix un parell {action, target} per a dissenyar i justificar la teva visualització (mira la diapositiva 42 de teoria).

L’objectiu principal de la visualització és resumir i explorar de manera intuïtiva informació pràctica i contextual de tots els països del món, especialment útil en un context de consulta ràpida o viatge internacional.

Els nostres objectius serien principalment relacionats amb la cerca, com:
- Comparar característiques bàsiques entre països (com la moneda, zona horària o codi de telèfon).
- Localitzar ràpidament informació útil a nivell global amb una representació espacial clara.
- Explorar patrons regionals (per exemple, països amb la mateixa moneda o franja horària).

El nostre {action, target} serian
- {action: "explore", target: "country attributes"} -> moure’s pel globus per descobrir informació de cada país de forma visual i dinàmica.

- {action: "compare", target: "regional groupings"} -> La visualització facilita comparacions entre regions/subregions per identificar patrons comuns (moneda, zona horària, etc.)

Aquest tipus de visualització permet una exploració intuïtiva, espacial i visual d’un conjunt de dades heterogeni però pràctic. S’adapta bé a l’objectiu perquè els atributs seleccionats són concrets i útils, i el mapa globus permet una navegació fluida entre països, ideal per a comparacions ràpides o consultes puntuals.


## 4. Defineixis un o més mappings visuals que et permetin visualitzar les dades de la millor manera possible, de forma justificada segons l’objectiu i les teves dades (How?).

Per visualitzar la informació de cada país sobre el globus, hem dissenyat tres tipus de representació complementària que permeten explorar, comparar i contextualitzar les dades de forma eficaç:

1. Punts geogràfics (point markers)

Cada país es representa amb un punt centrat en la seva latitud i longitud.

- Interacció: en passar el cursor, es mostra un tooltip o panell amb la info seleccionada
- Justificació: permet localitzar informació ràpida i concreta de cada país sense saturar visualment l’escena. És útil per a l’exploració puntual i individual de països.

2. Color per àrea segons la regió del país

Es pinta cada país amb un color segons una categoria discreta, com:

- Interacció: en passar el cursor, es mostra un tooltip o panell amb la info seleccionada
- Justificació: facilita la comparació regional i detecció de patrons, com veure quins països comparteixen moneda o zona horària. Ideal per a l’anàlisi comparativa global.

3. Contorn elevat (outline pop-up)

Molt semblant al anterior, pero ara es fa emfàsi en el contorn del país i de les fronteres d'aquests. 

- Interacció: en passar el cursor, es mostra un tooltip o panell amb la info seleccionada
- Justificació:  millora el focus visual, destacant un país d'interès sense perdre el context global. Aquesta tècnica afegeix profunditat i ajuda a centrar-se en països concrets en entorns amb molta densitat d’informació.

Aquestes tres representacions es poden combinar, ja que cobreixen necessitats visuals diferents:
- Explorar dades puntuals → punts amb tooltip.
- Detectar patrons regionals → colors per àrea.
- Focalitzar l’atenció → outline elevat i contorn animat.

En les tres visualitzacions, els colors s’assignen de forma categòrica segons el valor del camp seleccionat. Ens permet localitzar els països amb informació igual de manera más simple. A diferència de dades quantitatives (on faries servir gradients), aquí cada valor és únic i necessita un color clarament distint.


## Imatges de les diferents visualitzacions

### Currency per àrea

En aquesta visualització es mostra quines divises s’utilitzen segons la regió. Cada país està acolorit segons la seva moneda principal. És útil per identificar àrees geogràfiques amb la mateixa divisa (com la zona euro) i veure la diversitat de monedes al món.

![Currency vist per areas](/IMGreadme/currency_area.png)

### Continents per punts

Cada punt representa un país, ubicat segons les seves coordenades geogràfiques (latitud i longitud). El color del punt indica a quin continent pertany el país. Aquesta visualització permet veure com es distribueixen els països per continents, mantenint la ubicació real sobre el globus.

![Continents per punts ](/IMGreadme/points.png)

### Subcontinents per àrea

Aquí veiem els subcontinents representats per color segons la classificació geogràfica. Tot i que mostra bé la distribució per subregió, es perd la informació de la frontera o forma de cada país, ja que només es veu l’àrea general acolorida.

![Subregions vistes per areas](/IMGreadme/region_area.png)

### Subcontinents per outline

Visualització similar a l’anterior, però ara es mostra el contorn de cada país, cosa que permet veure les divisions polítiques entre estats dins de cada subregió. És més detallada i permet una millor identificació individual de països dins la mateixa subregió.

![Subregions vister per contorns](/IMGreadme/region_outline.png)
