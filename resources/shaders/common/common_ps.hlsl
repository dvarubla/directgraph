void clipEllipse(float2 position){
    clip(1.0f - position.x * position.x - position.y * position.y);
}

float4 sampleFillPattern(float4 colorIn0, float4 colorIn1, sampler texSampler, float2 tex){
    return lerp(colorIn0, colorIn1, tex2D(texSampler, tex).a);
}

//http://www.iquilezles.org/www/articles/ellipsedist/ellipsedist.htm
float ellipseDistance(float2 p, float2 ab){
    float dist;
    if(ab.y != ab.x){
        if(p.x > p.y){
            p=p.yx;
            ab=ab.yx;
        }
        float l = ab.y*ab.y - ab.x*ab.x;
        float m = ab.x*p.x/l; float m2 = m*m;
        float n = ab.y*p.y/l; float n2 = n*n;
        float c = (m2 + n2 - 1.0)/3.0;
        float c3 = c*c*c;
        float q = c3 + m2*n2*2.0;
        float d = c3 + m2*n2;
        float g = m + m*n2;

        float co;

        if(d<0.0)
        {
            float p = acos(min(q/c3, 0.99999))/3.0;
            float s = cos(p);
            float t = sin(p)*sqrt(3.0);
            float rx = sqrt( -c*(s + t + 2.0) + m2 );
            float ry = sqrt( -c*(s - t + 2.0) + m2 );
            co = ( ry + sign(l)*rx + abs(g)/(rx*ry) - m)/2.0;
        }
        else
        {
            float h = 2.0*m*n*sqrt( d );
            float s = sign(q+h)*pow( abs(q+h), 1.0/3.0 );
            float u = sign(q-h)*pow( abs(q-h), 1.0/3.0 );
            float rx = -s - u - c*4.0 + 2.0*m2;
            float ry = (s - u)*sqrt(3.0);
            float rm = sqrt( rx*rx + ry*ry );
            float p = ry/sqrt(rm-rx);
            co = (p + 2.0*g/rm - m)/2.0;
        }

        co = min(co, 1.0);
        float si = sqrt( 1.0 - co*co );
        float2 closestPoint = float2( ab.x*co, ab.y*si );
        dist = length(closestPoint - p) * sign(p.y - closestPoint.y);
    } else {
        dist = length(p) - ab.x;
    }
    return dist;
}