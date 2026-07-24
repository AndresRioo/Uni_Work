-- 
-- depends: 

CREATE TABLE IF NOT EXISTS drons (
    id UUID PRIMARY KEY,
    name VARCHAR(255) NOT NULL UNIQUE,
    latitude FLOAT NOT NULL,
    longitude FLOAT NOT NULL,
    status VARCHAR(50) NOT NULL DEFAULT 'disponible',
    joined_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    last_seen TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    battery_level FLOAT NOT NULL DEFAULT 100.0,
    last_telemetry TIMESTAMP,
    technical_info TEXT
);