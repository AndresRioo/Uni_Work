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

/**
 * Convert the population data into points
 * @param {object} data - The data to be formatted
 * @return {Promise<object>} - The data parsed into a dictionary
 */


/*
async function parseData(data) {
  let dataObject = {};
  
  // TODO: Canvieu aquest mètode per fer el parseig de les vostres dades

  // Format the data into a dictionary
  data.forEach(item => {
    const values = [];
    for (let i = 0; i < item[1].length; i += 3) {
      const point = {
        lat: item[1][i],
        lng: item[1][i + 1],
        population: item[1][i + 2]
      };
      values.push(point);
    }
    dataObject[item[0]] = values;
  });

  return dataObject;
}
  */

async function parseData(data) {
  let dataObject = {};

  data.forEach(item => {
    const countryName = item.name; // o item.iso2 si prefieres un código
    const point = {
      lat: parseFloat(item.latitude),
      lng: parseFloat(item.longitude),
      value: item.currency
    };
    dataObject[countryName] = point;
  });

  return dataObject;
}





/**
 * Read data from a file
 * @param {string} fileLocation - The location of the JSON file
 * @param {function} parseDataCallback - The data parse function
 * @return {Promise<object>} - The parsed data
 */
async function getDataFetch(fileLocation, parseDataCallback) {
  // We're going to ask a file for the JSON data.
  const response = await fetch(fileLocation);
  if (!response.ok) {
      throw new Error('File not found');
  }
  const data = await response.json();
  return await parseDataCallback(data);
}

// Convert a number into a simpler version. 
function formatShort(val) {
  if (val >= 1e9) return (val / 1e9).toFixed(1) + 'B';
  if (val >= 1e6) return (val / 1e6).toFixed(1) + 'M';
  return (val / 1e3).toFixed(1) + 'K';
}
